#pragma once
#include <iostream>
#include <functional>

template<typename T>
class AVLTree {
private:
    class Node {
    public:
        Node *left;
        Node *right;
        T value;
        int height;

        explicit Node(const T& v = T(), int h=1, Node *l=nullptr, Node *r=nullptr) : height(1) {
            value = v;
            left = l;
            right = r;
            height = h;
        }
    };

    Node *m_root;

    int height(Node *(&p));

    int bfactor(Node *(&p)); //разность высот правого и левого поддеревьев

    void set_height(Node *(&p));

    Node* L_rotate(Node *(&p));
    Node* R_rotate(Node *(&p));

    Node* balance(Node *(&p));

    Node* insert(const T &v, Node *(&p));

    Node* remove(const T &v, Node *(&p), bool *flag);
    Node* find_min(Node *(&p));
    Node* get_min(Node *(&p));

    bool contain(const T &v, Node *(&p));
    Node* find(const T &v, Node *&p);

    void where(std::function<bool(T)> func, Node *(&p), AVLTree<T>* res);
    void map(std::function<T(T)> func, Node *(&p), AVLTree<T>* res);
    void reduce(std::function<T(T, T)> func, Node *(&p), T& res);

    void get_subtree(const T &v, Node*(&p), AVLTree<T>* res, bool *exists);
    Node* copy(Node*(&p));
    void get_subtree_only_elements(const T &v, Node*(&p), AVLTree<T>* res, bool *exists);
    void insert_rec(Node*(&p), AVLTree<T>* res);

    bool is_subtree(Node*(&p1), Node*(&p2));

    bool check_balance(Node *(&p));

    void tree_to_string(Node *(&p), std::string& res);

    void print(Node *(&p));
    void print_level(Node *(&p), int level);
    void print_levels(Node *(&p));

    void get_heights(Node *(&p), std::vector<int>& res); //нужно для тестирования get_subtree и is_subtree

    void clear(Node *(&p));

public:
    AVLTree() { m_root = nullptr; } //создание пустого дерева

    ~AVLTree();

    int height();

    void insert(const T &value);
    bool remove(const T &value);
    bool contain(const T &value);
    bool empty();

    T& head() { return m_root->value; }

    T& minimum() { return find_min(m_root)->value; }

    AVLTree<T>* where(std::function<bool(T)> func);
    AVLTree<T>* map(std::function<T(T)> func);
    T& reduce(std::function<T(T, T)> func);

    AVLTree<T>* get_subtree(const T &v); //создается точная копия
    AVLTree<T>* get_subtree_only_elements(const T &v); //выделение поддерева происходит с помощью insert, поэтому
                                                       //возможно изменение структуры поддерева из-за балансировки

    bool is_subtree(AVLTree<T>& second); //проверяет, является ли поддерево точной копией
    bool is_equal(AVLTree<T>& second); //проверяет деревья на точное равенство (просто вызывает is_subtree для корней)

    bool check_balance();

    std::string tree_to_string();

    void print();
    void print_levels();

    void get_heights(std::vector<int>& res);

    void clear();
};

template<typename T>
AVLTree<T>::~AVLTree() {
    clear();
}

template<typename T>
int AVLTree<T>::height(Node *(&p)) {
    if (p == nullptr) {
        return 0;
    } else {
        return p->height;
    }
}

template<typename T>
int AVLTree<T>::bfactor(Node *(&p)) {
    if (p == nullptr) {
        return 0;
    } else {
        return height(p->right) - height(p->left);
    }
}

template<typename T>
void AVLTree<T>::set_height(Node *(&p)) {
    if (p != nullptr) {
        if (height(p->left) > height(p->right))
            p->height = height(p->left) + 1;
        else
            p->height = height(p->right) + 1;
    }
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::L_rotate(AVLTree::Node *&p) {
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    set_height(p);
    set_height(q);
    return q;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::R_rotate(AVLTree::Node *(&p)) {
    Node* q = p->right;
    p->right = q->left;
    q->left = p;
    set_height(p);
    set_height(q);
    return q;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::balance(AVLTree::Node *(&p)) {
    set_height(p);
    if (bfactor(p) > 1) {
        if (bfactor(p->right) < 0)
            p->right = L_rotate(p->right);
        return R_rotate(p);
    } else if (bfactor(p) < -1) {
        if (bfactor(p->left) > 0)
            p->left = R_rotate(p->left);
        return L_rotate(p);
    }
    return p;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::insert(const T &v, AVLTree::Node *(&p)) {
    if (p == nullptr) {
        p = new Node(v);
        return p;
    } else if (v < p->value)
        p->left = insert(v, p->left);
    else
        p->right = insert(v, p->right);
    return balance(p);
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::remove(const T &v, AVLTree::Node *(&p), bool *flag) { //flag нужен для того,
    if (p == nullptr) {                // чтобы сообщить об отсутствии удаляемого элемента в дереве
        *flag = false;
        return nullptr;
    }
    if (v < p->value)
        p->left = remove(v, p->left, flag);
    else if (v > p->value)
        p->right = remove(v, p->right, flag);
    else {
        Node* l = p->left;
        Node* r = p->right;
        delete p;
        if (r == nullptr)
            return l;
        Node* min = find_min(r); // На место удаленного элемента ставится самые левый элемент в правом поддереве.
        min->right = get_min(r); // find_min находит этот элемент, get_min "забирает" этот элемент, прикрепляет к
        min->left = l;              // родителю его правое поддерево (если оно есть).
        return balance(min);    // балансировка поддерева с корнем min
    }
    return balance(p); // на выходе из рекурсии каждое поддерево балансируется
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::find_min(AVLTree::Node *(&p)) {
    if (p->left != nullptr)
        return find_min(p->left);
    else
        return p;
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::get_min(AVLTree::Node *(&p)) {
    if (p->left == nullptr)
        return p->right;
    p->left = get_min(p->left);
    return balance(p);
}

template<typename T>
bool AVLTree<T>::remove(const T &value) {
    if (m_root == nullptr)
        //throw std::runtime_error("\nRemove Message : AVLTree is empty\n");
        return false;
    bool exists{true};
    m_root = remove(value, m_root, &exists);
    if (!exists) {
        //throw std::runtime_error("\nRemove Message : Element doesn't exist\n");
        return false;
    }
    return true;
}

template<typename T>
bool AVLTree<T>::contain(const T &v, Node *&p) {
    if (p == nullptr)
        return false;
    else if (p->value == v)
        return true;
    else if (v > p->value)
        return contain(v, p->right);
    else
        return contain(v, p->left);
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::find(const T &v, Node *&p) {
    if (p == nullptr)
        return nullptr;
    else if (p->value == v)
        return p;
    else if (v > p->value)
        return find(v, p->right);
    else
        return find(v, p->left);
}

template<typename T>
void AVLTree<T>::where(std::function<bool(T)> func, AVLTree::Node *&p, AVLTree<T>* res) {
    if (p != nullptr) {
        where(func, p->left, res);
        if (func(p->value)) {
            res->insert(p->value);
        }
        where(func, p->right, res);
    }
}

template<typename T>
AVLTree<T>* AVLTree<T>::where(std::function<bool(T)> func) {
    if (m_root == nullptr)
        throw std::runtime_error("\nWhere Message : AVLTree is empty\n");
    auto* res = new AVLTree<T>;
    where(func, m_root, res);
    return res;
}

template<typename T>
void AVLTree<T>::map(std::function<T(T)> func, AVLTree::Node *&p, AVLTree<T>* res) {
    if (p != nullptr) {
        map(func, p->left, res);
        res->insert(func(p->value));
        map(func, p->right, res);
    }
}

template<typename T>
AVLTree<T>* AVLTree<T>::map(std::function<T(T)> func) {
    if (m_root == nullptr)
        throw std::runtime_error("\nMap Message : AVLTree is empty\n");
    auto* res = new AVLTree<T>;
    map(func, m_root, res);
    return res;
}

template<typename T>
void AVLTree<T>::reduce(std::function<T(T, T)> func, AVLTree::Node *&p, T& res) {
    if (p != nullptr) {
        reduce(func, p->left, res);
        res = func(p->value, res);
        reduce(func, p->right, res);
    }
}

template<typename T>
T& AVLTree<T>::reduce(std::function<T(T, T)> func) {
    if (m_root == nullptr)
        throw std::runtime_error("\nReduce Message : AVLTree is empty\n");
    auto* res = new T{};
    reduce(func, m_root, *res);
    return *res;
}

template<typename T>
void AVLTree<T>::get_subtree_only_elements(const T &v, AVLTree::Node *&p, AVLTree<T>* res, bool *exists) {
    if (p != nullptr) {
        if (v > p->value)
            get_subtree(v, p->right, res, exists);
        else if (v < p->value)
            get_subtree(v, p->left, res, exists);
        else {
            insert_rec(p, res);
        }
    } else {
        *exists = false;
    }
}

template<typename T>
void AVLTree<T>::insert_rec(Node*(&p), AVLTree<T> *res) {
    if (p != nullptr) {
        insert_rec(p->left, res);
        res->insert(p->value);
        insert_rec(p->right, res);
    }
}

template<typename T>
AVLTree<T> *AVLTree<T>::get_subtree_only_elements(const T &v) {
    if (m_root == nullptr)
        throw std::runtime_error("\nGet_subtree_only_elements Message : AVLTree is empty\n");
    auto* res = new AVLTree<T>;
    bool exists{true};
    get_subtree_only_elements(v, m_root, res, &exists);
    if (!exists)
        throw std::runtime_error("\nGet_subtree_only_elements Message : Element doesn't exist\n");
    return res;
}

template<typename T>
void AVLTree<T>::get_subtree(const T &v, AVLTree::Node *&p, AVLTree<T> *res, bool *exists) {
    if (p != nullptr) {
        if (v > p->value)
            get_subtree(v, p->right, res, exists);
        else if (v < p->value)
            get_subtree(v, p->left, res, exists);
        else {
            res->m_root = copy(p);
        }
    } else {
        *exists = false;
    }
}

template<typename T>
typename AVLTree<T>::Node* AVLTree<T>::copy(AVLTree::Node *&p) {
    if (p == nullptr)
        return nullptr;
    Node* new_node = new Node(p->value, p->height);
    new_node->left = copy(p->left);
    new_node->right = copy(p->right);
    return new_node;
}

template<typename T>
AVLTree<T> *AVLTree<T>::get_subtree(const T &v) {
    if (m_root == nullptr)
        throw std::runtime_error("\nGet_subtree Message : AVLTree is empty\n");
    auto* res = new AVLTree<T>;
    bool exists{true};
    get_subtree(v, m_root, res, &exists);
    if (!exists)
        throw std::runtime_error("\nGet_subtree Message : Element doesn't exist\n");
    return res;
}

template<typename T>
bool AVLTree<T>::is_subtree(AVLTree::Node *&p1, AVLTree::Node *&p2) {
    if ((p1 == nullptr) && (p2 == nullptr))
        return true;
    if ((p1 == nullptr) || (p2 == nullptr))
        return false;
    return (p1->value == p2->value) && (is_subtree(p1->left, p2->left)) && (is_subtree(p1->right, p2->right));
}

template<typename T>
bool AVLTree<T>::is_subtree(AVLTree<T>& second) {
    if (m_root == nullptr)
        throw std::runtime_error("\nIs_subtree Message : AVLTree is empty\n");
    if (second.m_root == nullptr)
        throw std::runtime_error("\nIs_subtree Message : AVLTree (parameter) is empty\n");
    Node* p = find(second.m_root->value, m_root);
    if (p == nullptr)
        return false;
    return is_subtree(p, second.m_root);
}

template<typename T>
bool AVLTree<T>::is_equal(AVLTree<T>& second) {
    if (m_root == nullptr)
        throw std::runtime_error("\nIs_equal Message : AVLTree is empty\n");
    if (second.m_root == nullptr)
        throw std::runtime_error("\nIs_equal Message : AVLTree (parameter) is empty\n");
    return is_subtree(m_root, second.m_root);
}

template<typename T>
int AVLTree<T>::height() {
    return height(m_root);
}

template<typename T>
void AVLTree<T>::insert(const T &value) {
    m_root = insert(value, m_root);
}

template<typename T>
bool AVLTree<T>::contain(const T &value) {
    return contain(value, m_root);
}

template<typename T>
void AVLTree<T>::print() {
    if (m_root == nullptr)
        throw std::runtime_error("\nPrint Message : AVLTree is empty\n");
    print(m_root);
}

template<typename T>
void AVLTree<T>::print(Node *&p) {
    if (p != nullptr) {
        print(p->left);
        std::cout << p->value << " : " << p->height << " : " << bfactor(p) << "\n";
        print(p->right);
    }
}

template<typename T>
void AVLTree<T>::clear() {
    clear(m_root);
}

template<typename T>
void AVLTree<T>::clear(Node *&p) {
    if (p != nullptr) {
        clear(p->left);
        clear(p->right);
        delete p;
        p = nullptr;
    }
}

template<typename T>
bool AVLTree<T>::check_balance(AVLTree::Node *&p) {
    if (p == nullptr)
        return true;
    else if ((bfactor(p) < -1) || (bfactor(p) > 1))
        return false;
    else
        return check_balance(p->left) && check_balance(p->right);
}

template<typename T>
bool AVLTree<T>::check_balance() {
    if (m_root == nullptr)
        throw std::runtime_error("\nCheck_balance Message : AVLTree is empty\n");
    return check_balance(m_root);
}

template<typename T>
void AVLTree<T>::tree_to_string(AVLTree::Node *&p, std::string& res) {
    if (p != nullptr) {
        tree_to_string(p->left, res);
        res += std::to_string(p->value);
        tree_to_string(p->right, res);
    }
}

template<typename T>
std::string AVLTree<T>::tree_to_string() {
    if (m_root == nullptr)
        throw std::runtime_error("\nTree_to_string Message : AVLTree is empty\n");
    std::string res{};
    tree_to_string(m_root, res);
    return res;
}

template<typename T>
void AVLTree<T>::print_level(AVLTree::Node *&p, int level) {
    if (p != nullptr) {
        print_level(p->left, level - 1);
        if (level == 0) {
            std::cout << p->value << " ";
        }
        print_level(p->right, level - 1);
    }
}

template<typename T>
void AVLTree<T>::print_levels(AVLTree::Node *&p) {
    for (int level = 0; level <= p->height; ++level) {
        print_level(p, level);
        std::cout << "\n";
    }
}

template<typename T>
void AVLTree<T>::print_levels() {
    if (m_root == nullptr)
        throw std::runtime_error("\nPrint_levels Message : AVLTree is empty\n");
    print_levels(m_root);
}

template<typename T>
void AVLTree<T>::get_heights(AVLTree::Node *&p, std::vector<int>& res) {
    if (p != nullptr) {
        get_heights(p->left, res);
        res.push_back(height(p));
        get_heights(p->right, res);
    }
}

template<typename T>
void AVLTree<T>::get_heights(std::vector<int>& res) {
    if (m_root == nullptr)
        throw std::runtime_error("\nGet_heights Message : AVLTree is empty\n");
    get_heights(m_root, res);
}

template<typename T>
bool AVLTree<T>::empty() {
    if (m_root == nullptr) return true;
    else return false;
}