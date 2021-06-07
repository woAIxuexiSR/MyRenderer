#pragma once

#include "math/utility.hpp"
#include "gmm/gmm.hpp"
#include <queue>

class knncmp
{
    point p;
public:
    knncmp(const point& _p) : p(_p) {}
    bool operator()(const point& a, const point& b)
    {
        return (a - p).length_square() < (b - p).length_square();
    }
};


class kdTree
{
private:
    point p;    

    AXIS axis;
    int cnt;
    kdTree *left, *right;    
    kdTree *parent;

    void childset(std::vector<point>& out);
    void _knn(int k, const point& _p, std::priority_queue<point, std::vector<point>, knncmp>& pq) const;

public:
    kdTree() : parent(nullptr) {}
    kdTree(const point& _p, AXIS _a) : p(_p), axis(_a), cnt(1), left(nullptr), right(nullptr), parent(nullptr) {}
    kdTree(std::vector<point>& data) : kdTree(data, 0, data.size(), (AXIS)random_int(0, 2)) {}
    kdTree(std::vector<point>& data, int start, int end, AXIS _a);
    
    ~kdTree();

    void rebuild(const std::vector<point>& data);
    void insert(const point& _p);
    std::vector<point> knn(int k, const point& _p) const;
};

#include "kdTree.inl"