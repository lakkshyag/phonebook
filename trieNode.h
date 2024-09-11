#include <vector>

class trieNode
{//node for our trie
    public:
    char data;
    trieNode** children;                                    //an array which will contains addresses of other trienodes
    int numChildren;                                        //number of possible children, can be 10 (if this is a number dict) for 256 (if this is a name dict)
    bool isTerminal;
    vector<contactNode*>* belongsTo;                        //vector which contains all the possilbe link list nodes a name/number can belogn tos

    trieNode(char c, int n)
    {//constructor
        data = c;
        numChildren = n;
        children = new trieNode*[numChildren];
        
        int i = 0;
        while ( i <= numChildren-1 )
        {
            *(children+i) = NULL;
            i++;
        }
        isTerminal = false;
        belongsTo = new vector<contactNode*>;
    }

    ~trieNode()
    {//destructor
        delete[] children;
        delete belongsTo;
    }
};

class trie
{//actual class for out trie
    trieNode* root;
    int numChildren;

    public:
    trie(int n)
    {
        numChildren = n;
        if ( numChildren == 256 )
        {
            cout<<"ascii trie created..."<<endl;
        }
        else if ( numChildren == 10 )
        {
            cout<<"number trie created..."<<endl;
        }
        else
        {
            cout<<"something wrong happened..."<<endl;
        }
        root = new trieNode('\0', numChildren);
    }

    ~trie()
    {
        int i = 0;
        while ( i <= numChildren-1)
        {
            delete (*root).children[i];
            i++;
        }
        delete root;
    }

    void insertWord(trieNode* root, string word, contactNode* node)
    {//function to insert a word in the trie
        if ( word.length() == 0 )
        {
            (*root).isTerminal = true;
            (*(*root).belongsTo).push_back(node);

            // cout<<"inserted in "<<(*node).index<<endl;
            return;
        }

        trieNode* child;
        int index = word[0];
        if ( numChildren == 10 )
        {
            index -= '0';
        }
        if ( (*root).children[index] != NULL )
        {
            child = (*root).children[index];
        }
        else
        {
            child = new trieNode(word[0], numChildren);
            (*root).children[index] = child;
        }
        insertWord(child, word.substr(1), node);
    }

    void insertWord(string word, contactNode* node)
    {//helper function
        insertWord(root, word, node);
    }

    void removeWord(trieNode* root, string word, contactNode* node)
    {//function to remove a word from the trie
        if ( word.length() == 0 )
        {
            int i = 0;
            int n = (*(*root).belongsTo).size();

            if ( n == 1 )
            {
                //cout<<"removed from dictionary, vec len 0"<<endl;
                (*(*root).belongsTo).pop_back();
                (*root).isTerminal = false;
                return;
            }
            else
            {
                while ( i <= n-1 )
                {
                    contactNode* toRemove = (*(*root).belongsTo).at(i);
                    if ( toRemove == node )
                    {
                        (*(*root).belongsTo).erase((*(*root).belongsTo).begin() + i);
                        //cout<<"removed from dictionary, vec len "<<(*(*root).belongsTo).size()<<endl;
                        return;
                    }        
                    i++;
                }
            }
            // cout<<"no word removed..."<<endl;
            return;
        }

        trieNode* child;
        int index = word[0];
        if ( numChildren == 10 )
        {
            index -= '0';
        }
        if ( (*root).children[index] != NULL )
        {
            child = (*root).children[index];
        }
        else
        {
            cout<<"word not matching, cant remove"<<endl;
            return;
        }

        removeWord(child, word.substr(1), node);

        if ( (*child).isTerminal == false )
        {
            int i = 0;
            while ( i <= numChildren-1 )
            {
                if ( (*child).children[i] != NULL )
                {
                    return;
                }
                i++;
            }
            delete child;
            (*root).children[index] = NULL;
        }

    }

    void removeWord(string word, contactNode* node)
    {//helper function
        removeWord(root, word, node);
    }

    void allPrinter(trieNode* root)
    {//function to print all words stored starting at root
        if ( root == NULL )
        {
            return;
        }

        if ( (*root).isTerminal )
        {
            int i = 0;
            int n = (*(*root).belongsTo).size();

            while ( i <= n-1 )
            {
                contactNode* node = (*(*root).belongsTo).at(i);

                cout<<"Index: "<<(*node).index<<endl;
                cout<<"Number: "<<(*node).phoneNo<<endl;
                cout<<"Name: "<<(*node).name<<endl;
                
                string email = (*node).email;
                if ( email != "~" )
                {
                    cout<<"Email: "<<(*node).email<<endl;
                }
                cout<<endl;

                i++; 
            }
        }
    
        int i = 0;
        while ( i <= numChildren-1 )
        {
            trieNode* child = (*root).children[i];
            if ( child != NULL )
            {
                char c = (*child).data;
                allPrinter(child);
            }
            i++;
        }
        return;
    }

    void allPrinter()
    {
        allPrinter(root);
    }

    void autoCompleter(trieNode* root, string input)
    {//function which auto completes searches based on current input
        if ( input.size() == 0 )
        {
            allPrinter(root);
            return;
        }
        else
        {
            int index = input[0];
            if ( numChildren == 10 )
            {
                index -= '0';
            }
            trieNode* child = (*root).children[index];

            if ( child != NULL )
            {
                autoCompleter(child, input.substr(1));
            }
            return;
        }
    }

    void autoCompleter(string input)
    {//helper
        autoCompleter(root, input);
    }
};