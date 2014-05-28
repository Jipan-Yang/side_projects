class LRUCache{
    int cap=0;
    
    //doubly linked list with key and value stored
    struct dll_node {
        int key;
        int val;
        dll_node *pre, *next;
        dll_node(int k, int v):key(k), val(v), pre(NULL), next(NULL) {}
    };
    
    dll_node *head=NULL, *tail=NULL;
    unordered_map<int, dll_node *> htbl;
    
    //move the node as new head
    void mv2head(dll_node *node) {
        if(head == node)
            return;
        
        if(tail == node){
            node->pre->next=NULL;
            tail = node->pre;
        }
        else {
            node->pre->next = node->next;
            node->next->pre = node->pre;
        }
        node->pre = NULL;
        head->pre = node;
        node->next = head;
        head = node;
    }
    
    //remove old tail
    void rmvTail(){
        dll_node *t = tail->pre;
        
        htbl.erase(tail->key);
        if(head == tail) {
            delete head;
            head = NULL;
            tail = NULL;
            return;
        }
        
        t->next = NULL;
        delete tail;
        tail = t;
    }
    
    //insert node as new head
    void insertNode(int key, int val){
        dll_node *p = new dll_node(key, val);
        htbl[key] = p;
        if(head == NULL) {
            head = p;
            tail = p;
            return;
        }
        p->next = head;
        head->pre = p;
        head = p;
    }
public:
    LRUCache(int capacity) {
        cap = capacity;
    }
    
    int get(int key) {
        if(htbl.find(key) == htbl.end())
            return -1;
        
        mv2head(htbl[key]);
        return htbl[key]->val;
    }
    
    void set(int key, int value) {
        if(get(key) != -1) {
            htbl[key]->val = value;   
            return;
        }
        if(cap == 0)
            return;
        if(htbl.size() == cap) {
            rmvTail();
        }
        insertNode(key, value);
    }
};
