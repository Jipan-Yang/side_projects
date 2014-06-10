/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
    ListNode *mergeList(ListNode *p1, ListNode *p2)
    {
        ListNode dummy(-1);
        ListNode *pre = &dummy;
        
        while(p1 && p2){
            if(p1->val < p2->val) {
                pre->next = p1;
                p1 = p1->next;
            }
            else {
                pre->next = p2;
                p2 = p2->next;
            }
            pre = pre->next;
        }
        
        p1 = p1?p1:p2;
        
        pre->next = p1;
        return dummy.next;
    }
    void splitList(ListNode *head, ListNode* &p2)
    {
        ListNode *pre=NULL;
        
        p2 = head;
        
        while(p2 != NULL && p2->next != NULL){
            pre = head;
            head = head->next;
            p2 = p2->next->next;
        }
        
        p2 = head;
        pre->next = NULL;
    }
public:
    ListNode *sortList(ListNode *head) {
        if(head == NULL || head->next == NULL)
            return head;
            
        ListNode  *p2;
        splitList(head, p2);
        head = sortList(head);
        p2 = sortList(p2);
        return mergeList(head, p2);
    }
};