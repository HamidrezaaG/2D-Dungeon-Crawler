#pragma once

class TileMap;

struct PathNode
{
    enum PathNodeStatus
    {
        Unchecked,
        Open,
        Closed,
    };

    int parentNodeIndex;
    PathNodeStatus status;

    float f; // Current sum.
    float g; // Cost to get to current node.
    float h; // Heuristic: manhatten distance to destination.
};

class Pathfinder
{
protected:

    IVector2 m_MapDimensions;
    TileMap* m_pTilemap;

    PathNode* m_Nodes;
    int m_NumNodes;

    int* m_OpenNodes;
    int m_NumOpen;

    // Internal methods.
    void AddToOpen(int nodeindex);
    void RemoveFromOpen(int nodeindex);
    int FindNodeIndexWithLowestFInOpen();

    int CalculateNodeIndex(int tx, int ty);
    int CheckIfNodeIsClearAndReturnNodeIndex(int tx, int ty);
    void AddNeighboursToOpenList(int nodeIndex, int endNodeIndex);

    float CalculateH(int nodeIndex1, int nodeIndex2);

public:

    TileMap* GetTilemap();
    Pathfinder(TileMap* pTilemap);
    virtual ~Pathfinder();

    void Reset();

    // Start a search, supply the starting tile x,y and ending tile x,y.
    // Returns true if path found, false otherwise.
    bool FindPath(int sx, int sy, int ex, int ey);

    // Retrieve the final path, pass a nullptr for "path" to just get length of the path.
    int GetPath(int* path, int maxdistance, int ex, int ey);
};

