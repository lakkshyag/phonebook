using namespace std;

class contactNode
{//the node of the LL which will store the contact information 
    public:
    int index;
    string name;
    unsigned long long phoneNo;
    string email;
    contactNode* next;


    public:
    contactNode(int index, string name, unsigned long long phoneNo, string email = "~")
    {//constructor
        (*this).index = index;
        (*this).name = name;
        (*this).phoneNo = phoneNo;
        (*this).email = email;
        next = NULL;
    }
};