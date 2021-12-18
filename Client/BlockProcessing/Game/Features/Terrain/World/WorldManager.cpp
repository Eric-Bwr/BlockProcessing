#include "WorldManager.h"
#include <mutex>
#include <condition_variable>
#include <algorithm>

static AsyncLoader<OctreeNode*> loader = AsyncLoader<OctreeNode*>(1);
static std::mutex octreeAccess;


struct ChunkGen{
	static std::atomic_int totalCount;
	std::shared_ptr<WorldManager> ptr;
	OctreeNode* candidate;
	int index;
	ChunkGen(const std::shared_ptr<WorldManager>& ptr, OctreeNode* candidate) : ptr(ptr), candidate(candidate), index(totalCount++) {
		print("ChunkGen constructor called " + std::to_string(index));
	}
	~ChunkGen(){
		print("ChunkGen destructor called " + std::to_string(index));
	}
	ChunkGen(const ChunkGen&) = delete;
	ChunkGen& operator=(const ChunkGen&) = delete;

	ChunkGen(ChunkGen&& other) noexcept{
		ptr = std::move(other.ptr);
		candidate = other.candidate;
		index = other.index;
		other.index = -other.index;
		print("ChunkGen move constructor called " + std::to_string(index));
	}

	ChunkGen& operator=(ChunkGen&& other) noexcept{
		ptr = std::move(other.ptr);
		candidate = other.candidate;
		index = other.index;
		other.index = -other.index;
		print("ChunkGen move assign called " + std::to_string(index));
		return *this;
	}

	OctreeNode* operator()() const{
		ptr->chunkManager.generateChunkData(candidate->chunk);
		return candidate;
	}
};

std::atomic_int ChunkGen::totalCount = 0;

struct OctreeGen{
	static std::atomic_int totalCount;
	std::shared_ptr<WorldManager> ptr;
	Coord playerChunkCoord;
	int idlingGenerators;
	int index;
	OctreeGen(const std::shared_ptr<WorldManager>& ptr, Coord playerChunkCoord, int idlingGenerators) : ptr(ptr), playerChunkCoord(playerChunkCoord), idlingGenerators(idlingGenerators), index(totalCount++){
		print("OctreeGen constructor called " + std::to_string(index));
	}
	~OctreeGen(){
		print("OctreeGen destructor called " + std::to_string(index));
	}
	OctreeGen(const OctreeGen&) = delete;
	OctreeGen& operator=(const OctreeGen&) = delete;

	OctreeGen(OctreeGen&& other) noexcept {
		ptr = std::move(other.ptr);
		playerChunkCoord = other.playerChunkCoord;
		idlingGenerators = other.idlingGenerators;
		index = other.index;
		other.index = -other.index;
		print("OctreeGen move constructor called " + std::to_string(index));
	}

	OctreeGen& operator=(OctreeGen&& other) noexcept{
		ptr = std::move(other.ptr);
		playerChunkCoord = other.playerChunkCoord;
		idlingGenerators = other.idlingGenerators;
		index = other.index;
		other.index = -other.index;
		print("OctreeGen move assign called " + std::to_string(index));
		return *this;
	}

 void operator()() const{
		ptr->chunkCandidatesForGenerating.clear();
//	for (auto coord : ptr->modifiedChunks) {
//		Coord coords[7] = {coord,
//						   {coord.x, coord.y + 1, coord.z},
//						   {coord.x, coord.y - 1, coord.z},
//						   {coord.x + 1, coord.y, coord.z},
//						   {coord.x - 1, coord.y, coord.z},
//						   {coord.x, coord.y, coord.z + 1},
//						   {coord.x, coord.y, coord.z - 1}};
//		for (auto chunkCoord : coords) {
//			auto leaf = ptr->octrees.find(getOctreeFromChunk(chunkCoord))->second->getRoot().getLeaf(chunkCoord);
//			leaf->chunk->generating = true;
//			ptr->chunkCandidatesForGenerating.push_back(leaf);
//		}
//	}
//	ptr->modifiedChunks.clear();

		auto playerOctreeCoord = getOctreeFromChunk(playerChunkCoord);
		auto octreeRadius = ptr->octreeRadius;
		for (int64_t xx = playerOctreeCoord.x - octreeRadius;
				xx <= playerOctreeCoord.x + octreeRadius; xx += OCTREE_LENGTH) {
			for (int64_t yy = playerOctreeCoord.y - octreeRadius;
					yy <= playerOctreeCoord.y + octreeRadius; yy +=
							OCTREE_LENGTH) {
				for (int64_t zz = playerOctreeCoord.z - octreeRadius;
						zz <= playerOctreeCoord.z + octreeRadius; zz +=
								OCTREE_LENGTH) {
					auto octreeCoord = Coord { xx, yy, zz };
					Octree *octree = nullptr;
					{
						std::lock_guard<std::mutex> lock(octreeAccess);
						auto it = ptr->octrees.find(octreeCoord);
						if (it == ptr->octrees.end()) {
							octree = new Octree(&ptr->frustum,
									&ptr->chunkManager, octreeCoord);
							octree->updateProperties(ptr->chunkingRadiusSquared,
									ptr->chunkingDeletionRadiusSquared);
							ptr->octrees.insert(
									std::pair<Coord, Octree*>(octreeCoord,
											octree));
						} else {
							octree = it->second.get();
						}
					}
					if (octree->getRoot().loadedChildren != 0b11111111)
						octree->getRoot().getClosestUnloadedChunks(
								ptr->chunkCandidatesForGenerating,
								idlingGenerators, playerChunkCoord);
				}
			}
		}

		for (auto candidate : ptr->chunkCandidatesForGenerating) {
			candidate->chunk->generating = true;

			ChunkGen gen(ptr, candidate);
			loader.scheduleTask([gen=std::move(gen)]() mutable {
				return gen();
			});
		}
		ptr->finishedUpdatingOctree = true;
	}
};

std::atomic_int OctreeGen::totalCount = 0;

void WorldManager::init(BlockManager *blockManager, std::shared_ptr<WorldManager>* ptrToThis) {
	chunkManager.init(blockManager, this);
	this->ptrToThis = ptrToThis;
}

void WorldManager::generate(const Coord &playerChunkCoord) {
	bool success = true;
	while (success) {
		OctreeNode *result = nullptr;
		loader.getResult(result, std::chrono::milliseconds(0), success);
		if (success) {
			if (result->chunk->init)
				chunkManager.initChunk(result->chunk);
			chunkManager.loadChunkData(result->chunk);
			result->updateHierarchyLoadedAndEmptyStatus(true,
					result->chunk->vertexCount == 0);
			result->chunk->generating = false;
		}
	}

	for (auto it = octrees.begin(), next_it = it; it != octrees.end(); it =
			next_it) {
		++next_it;
		it->second->getRoot().deleteFurthestLoadedChunks(playerChunkCoord);
		if (Coord::distanceSquared(playerChunkCoord,
				it->second->getRoot().coord) >= octreeDeletionRadiusSquared)
			octrees.erase(it);
	}

	int idlingGenerators = maxPendingJobs - loader.getItemsCount();
	if (idlingGenerators <= 0)
		return;

	if (!finishedUpdatingOctree)
		return;

	finishedUpdatingOctree = false;

	OctreeGen gen(*ptrToThis, playerChunkCoord, idlingGenerators);

	loader.exec([gen=std::move(gen)]() mutable {
		return gen();
	});
}

Chunk* WorldManager::getChunkFromBlockCoords(int64_t x, int64_t y, int64_t z) {
	std::lock_guard<std::mutex> lock(octreeAccess);

	auto chunk = getChunkFromBlock( { x, y, z });
	auto it = octrees.find(getOctreeFromChunk(chunk));
	if (it == octrees.end())
		return nullptr;
	return it->second->getRoot().getLeaf(chunk)->chunk;
}

Chunk* WorldManager::getChunkFromChunkCoords(int64_t x, int64_t y, int64_t z) {
	std::lock_guard<std::mutex> lock(octreeAccess);

	auto chunk = Coord { x, y, z };
	auto it = octrees.find(getOctreeFromChunk(chunk));
	if (it == octrees.end())
		return nullptr;
	return it->second->getRoot().getLeaf(chunk)->chunk;
}

int8_t WorldManager::getBlockDefault(int64_t x, int64_t y, int64_t z) {
	float threshold = 0.55;
	auto noise = (fastNoise->GetNoise(x, y, z) + 1.0f) / 2.0f;
	if (noise < threshold)
		return BLOCK_AIR;
	else {
		if ((fastNoise->GetNoise(x, y + 1, z) + 1.0f) / 2.0f < threshold) {
			return BLOCK_GRASS;
		} else if ((fastNoise->GetNoise(x, y + 2, z) + 1.0f) / 2.0f
				< threshold) {
			return BLOCK_DIRT;
		} else
			return BLOCK_STONE;
	}
	//auto second = (fastNoise->GetNoise(x, y, z) + 1.0f) / 2.0f;
	//if(second > 0.65)
	//    return BLOCK_PLANKS;
	//int height = int(((fastNoise->GetNoise(x, z) + 1.0f) / 2.0f) * 200);
	//if (y > height || y < 0) {
	//    return BLOCK_AIR;
	//} else if (y == height) {
	//    return BLOCK_GRASS;
	//} else if (y < height && y >= height - 1) {
	//    return BLOCK_DIRT;
	//} else {
	//    return BLOCK_STONE;
	//}
}

int8_t WorldManager::getBlock(int64_t x, int64_t y, int64_t z) {
	std::lock_guard<std::mutex> lock(octreeAccess);

	auto chunkCoord = getChunkFromBlock( { x, y, z });
	auto it = octrees.find(getOctreeFromChunk(chunkCoord));
	if (it != octrees.end()) {
		auto chunk = it->second->getRoot().getLeaf(chunkCoord)->chunk;
		if (chunk != nullptr)
			return chunkManager.getChunkBlock(chunk, x, y, z);
	}
	return getBlockDefault(x, y, z);
}

void WorldManager::setBlock(int8_t block, int64_t x, int64_t y, int64_t z) {
	std::lock_guard<std::mutex> lock(octreeAccess);

	auto chunkCoord = getChunkFromBlock( { x, y, z });
	auto it = octrees.find(getOctreeFromChunk(chunkCoord));
	if (it != octrees.end()) {
		auto leaf = it->second->getRoot().getLeaf(chunkCoord);
		if (leaf->chunk != nullptr) {
			chunkManager.setChunkBlock(leaf->chunk, block, x, y, z);
			modifiedChunks.push_back(leaf->coord);
		}
	}
}

void WorldManager::setBlocks(const std::vector<int8_t> &blocks, int64_t x,
		int64_t y, int64_t z) {
	std::lock_guard<std::mutex> lock(octreeAccess);

	Coord chunkCoord = { x, y, z };
	auto it = octrees.find(getOctreeFromChunk(chunkCoord));
	if (it != octrees.end()) {
		auto leaf = it->second->getRoot().getLeaf(chunkCoord);
		if (leaf->chunk != nullptr) {
			leaf->chunk->blocks = blocks;
			modifiedChunks.push_back(leaf->coord);
		}
	}
}

void WorldManager::updateChunkFromBlockCoords(int64_t x, int64_t y, int64_t z) {
	modifiedChunks.push_back(getChunkFromBlock( { x, y, z }));
}

void WorldManager::updateChunkFromChunkCoords(int64_t x, int64_t y, int64_t z) {
	modifiedChunks.push_back( { x, y, z });
}

void WorldManager::setChunkingRadius(int radius) {
	std::lock_guard<std::mutex> lock(octreeAccess);

	this->octreeRadius = ((radius / OCTREE_LENGTH) + 1) * OCTREE_LENGTH;
	this->octreeDeletionRadiusSquared = (octreeRadius + 2.5 * OCTREE_LENGTH)
			* (octreeRadius + 2.5 * OCTREE_LENGTH);
	this->chunkingRadiusSquared = radius * radius;
	this->chunkingDeletionRadiusSquared = (radius + 2) * (radius + 2);
	for (auto& [coord, octree] : octrees)
		octree->updateProperties(chunkingRadiusSquared,
				chunkingDeletionRadiusSquared);
}

void WorldManager::setChunksPerThread(int max) {
	this->maxPendingJobs = (std::thread::hardware_concurrency()) * max;
}

void WorldManager::render(Mat4d &projectionView, Mat4d &view) {
	std::lock_guard<std::mutex> lock(octreeAccess);
	frustum.update(projectionView);
	chunkManager.setView(view);
	for (auto& [coord, octree] : octrees)
		octree->getRoot().render();
}

WorldManager::~WorldManager() {
	std::lock_guard<std::mutex> lock(octreeAccess);
	octrees.clear();
	delete fastNoise;
}
