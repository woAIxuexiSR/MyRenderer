#include "kdTree.hpp"

bool point_cmp_x(const point& _a, const point& _b)
{
    return _a.x < _b.x;
}

bool point_cmp_y(const point& _a, const point& _b)
{
    return _a.y < _b.y;
}

bool point_cmp_z(const point& _a, const point& _b)
{
    return _a.z < _b.z;
}

// [start, end)
kdTree::kdTree(std::vector<point>& data, int start, int end, AXIS _a) : axis(_a), cnt(end - start), parent(nullptr)
{
    if(end - start == 1)
    {
        p = data[start];
        left = right = nullptr;
        return;
    }

    auto cmp = (_a == AXIS::AXIS_X) ? point_cmp_x
             : (_a == AXIS::AXIS_Y) ? point_cmp_y
                                    : point_cmp_z;
    
    std::sort(data.begin() + start, data.begin() + end, cmp);

    int mid = (start + end) / 2;
    
    p = data[mid];
    left = right = nullptr;
    if(mid > start) left = new kdTree(data, start, mid, next_axis(_a));
    if(end > mid + 1) right = new kdTree(data, mid + 1, end, next_axis(_a));

    if(left) left->parent = this;
    if(right) right->parent = this;
}

kdTree::~kdTree()
{
    if(left) delete left;
    if(right) delete right;
}

void kdTree::childset(std::vector<point>& out)
{
    out.push_back(p);
    if(left) left->childset(out);
    if(right) right->childset(out);
}

void kdTree::_knn(int k, const point& _p, std::priority_queue<point, std::vector<point>, knncmp>& pq) const
{
    bool lt = (axis == AXIS::AXIS_X) ? (_p.x < p.x)
            : (axis == AXIS::AXIS_Y) ? (_p.y < p.y)
                                     : (_p.z < p.z);

    kdTree* node = lt ? left : right;
    if(node) node->_knn(k, _p, pq);

    pq.push(p);
    if(pq.size() > k) pq.pop();

    node = lt ? right : left;
    if(node)
    {
        double dis = (axis == AXIS::AXIS_X) ? fabs(_p.x - p.x)
                   : (axis == AXIS::AXIS_Y) ? fabs(_p.y - p.y)
                                            : fabs(_p.z - p.z);

        point t = pq.top();
        if((t - _p).length_square() > dis * dis)
            node->_knn(k, _p, pq);
    }
}

void kdTree::rebuild(const std::vector<point>& data)
{
    std::vector<point> t;
    childset(t);
    t.insert(t.end(), data.begin(), data.end());

    if(left) delete left;
    if(right) delete right;

    *this = kdTree(t);
}

void kdTree::insert(const point& _p)
{
    cnt++;

    bool lt = (axis == AXIS::AXIS_X) ? (_p.x < p.x)
            : (axis == AXIS::AXIS_Y) ? (_p.y < p.y)
                                     : (_p.z < p.z);
    
    int ln = left ? left->cnt : 0;
    int rn = right ? right->cnt : 0;

    double prop = (double) (lt ? ln + 1 : ln) / (ln + rn + 1);
    if(prop < 0.3 || prop > 0.7)
    {
        std::vector<point> childs { _p };
        childset(childs);

        auto cmp = (axis == AXIS::AXIS_X) ? point_cmp_x
                 : (axis == AXIS::AXIS_Y) ? point_cmp_y
                                          : point_cmp_z;

        std::sort(childs.begin(), childs.end(), cmp);

        int n = childs.size();
        int mid = n / 2;
        
        p = childs[mid];

        if(left) delete left;
        if(right) delete right;

        if(mid > 0) left = new kdTree(childs, 0, mid, next_axis(axis));
        if(n > mid + 1) right = new kdTree(childs, mid + 1, n, next_axis(axis));

        if(left) left->parent = this;
        if(right) right->parent = this;
    }
    else
    {
        kdTree* node = lt ? left : right;
        if(node) node->insert(_p);
        else
        {
            if(lt) 
            {
                left = new kdTree(_p, next_axis(axis));
                left->parent = this;
            }
            else
            {
                right = new kdTree(_p, next_axis(axis));
                right->parent = this;
            }
        }
    }
}

std::vector<point> kdTree::knn(int k, const point& _p) const
{
    knncmp mycmp(_p);
    std::priority_queue<point, std::vector<point>, knncmp> pq(mycmp);

    _knn(k, _p, pq);

    std::vector<point> out;
    while(!pq.empty())
    {
        out.push_back(pq.top());
        pq.pop();
    }

    return out;
}