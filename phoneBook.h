#include <fstream>                                                          //fstream allows to read and write to and from text files
using namespace std;

unsigned long long strtoULL(string input)                                  
{ //function which converts a string into an unsigned long long
    unsigned long long phoneNo = 0;
    int i = 0;
    while ( input[i] != '\0' )
    {
        if ( i >= 15 )
        {
            cout<<"Invalid phone no."<<endl;
            return 0;
        }
        if ( ( input[i]-'0' >= 10 ) || ( input[i] - '0' <= -1 ) )
        {
            cout<<"Invalid phone no."<<endl;
            return 0;
        }
        phoneNo *= 10;
        phoneNo += input[i] - '0';
        i++;
    }

    return phoneNo;
}

string ULLtoStr(unsigned long long num)                                    
{//function which coverts an unsigned long long into a string
    string ans = "";
    while ( num > 0 )
    {
        int digit = num%10;
        char c = '0' + digit;

        ans = c + ans;
        num /= 10;
    }
    return ans;
}

string convertToLowercase(string input)
{//function which changes every alphabet to lowercase
    int len = input.length();
    int i = 0;
    while ( i <= len-1 )
    {
        input[i] = tolower(input[i]);
        i++;
    }

    return input;
}

class phoneBook 
{//a class which contains everything regarding the phonebook
    public: 
    string folderName;
    string contactFile;
    string username;
    string password;                                //user information

    int numberContacts;
    contactNode* head;
    contactNode* tail;                              //linked list information

    int modification;
    int addition;
    int deletion;
    int imports;                                   //to keep track of changes made

    trie* dictName;
    trie* dictNum;                                 //dictionary for storing the names and phone numbers for easier access in a trie

    phoneBook(string username, string password)
    {
        head = NULL;
        tail = NULL;
        (*this).username = username;
        (*this).password = password;
        folderName = "./contactInfo/";
        contactFile = folderName+username+"Contacts.txt";
        
        numberContacts = 0;
        modification = 0;
        addition = 0;
        deletion = 0;
        imports = 0;

        dictName = new trie(256);                         //names can contain any of the possible letter so I took 256 for all possible ASCII values
        dictNum = new trie(10);                           //numbers can contain 0-9
    }

    contactNode* purgeLL(contactNode* head)
    {//function to delete the entire LL
        if ( head == NULL )
        {
            return NULL;
        }
        else
        {
            (*head).next = purgeLL((*head).next);
            delete head;
            return NULL;
        }
    }

    ~phoneBook()
    {//destructor
        head = purgeLL(head);
        tail = head;
        numberContacts = 0;
        folderName = "";
        contactFile = "";

        delete dictName;
        delete dictNum;

        cout<<"Phonebook closed."<<endl;
    }

    void display(contactNode* temp)
    {//a function to display a contact based on its index.
        int index = (*temp).index;
        string name = (*temp).name;
        unsigned long long phoneNo = (*temp).phoneNo;
        string email = (*temp).email;

        cout<<"Index: "<<index<<endl;
        cout<<"Name: "<<name<<endl;
        cout<<"Phone No.: "<<phoneNo<<endl;

        if ( email != "~" )
        {

            cout<<"Email: "<<email<<endl;
        }

        cout<<"____________________________________________"<<endl;
    }

    contactNode* createContactFile()
    {//a function to create a LL from the txt file
        ifstream contactFileRead(contactFile, ios::in);
        contactNode* head = NULL;
        contactNode* tail = NULL;
        int index = 0;

        if ( !contactFileRead.is_open() )
        {
            cout<<"contacts.txt unable to be opened..."<<endl;
        }
        else
        {
            while (!contactFileRead.eof())
            {
                string name = "";
                unsigned long long phoneNo = 0;
                string email = "";
    
                getline(contactFileRead, name, '\n');
                
                string tempNo;
                getline(contactFileRead, tempNo, '\n');            
                phoneNo = strtoULL(tempNo);
                
                getline(contactFileRead, email, '\n');

                if ( phoneNo )
                {
                    contactNode* newNode = new contactNode(index, name, phoneNo, email);
                    if ( tail == NULL )
                    {
                        head = newNode;
                        tail = head; 
                    }
                    else
                    {
                        (*tail).next = newNode;
                        tail = newNode;
                    }

                    (*dictName).insertWord(name, newNode);
                    (*dictNum).insertWord(tempNo, newNode);
                    index++;
                }
                else
                {
                    cout<<"unchanged input or phoneNo entered as 0..."<<endl;
                }                
            }
            (*this).head = head;
            (*this).tail = tail;
            numberContacts = index;
        }
        contactFileRead.close();    

        return head;
    }

    void saveContactFile()
    {//function to write the LL into the txt file
        ofstream contactFileWrite(contactFile, ios::trunc);

        if ( !contactFileWrite.is_open() )
        {
            cout<<"contact.txt unable to be opened..."<<endl;
        }
        else
        {
            contactNode* temp = head;
            while ( temp )
            {
                string name = (*temp).name;
                unsigned long long phoneNo = (*temp).phoneNo;
                string email = (*temp).email;

                contactFileWrite<<name<<endl;
                contactFileWrite<<phoneNo<<endl;
                contactFileWrite<<email<<endl;

                temp = (*temp).next;
            }
        }
        contactFileWrite.close();

    }

    void displayContactsAll()
    {//function to display all the contacts
        if ( numberContacts == 0 )
        {
            cout<<"No contacts to display."<<endl;
            return;
        }
        cout<<"Printing all contacts."<<endl;
        cout<<"___________________________________________________________________________________"<<endl<<endl;
        contactNode* temp = head;
        
        while ( temp )
        {
            display(temp);
            temp = (*temp).next;
        }
    }

    void deleteContactsAll()
    {//a function to delete all contacts
        head = purgeLL(head);
        tail = head;

        deletion += numberContacts;
        numberContacts = 0;
        
        cout<<"All contacts deleted."<<endl;
    }

    void indexReassign()
    {//a function to properly reassign indices, since deleting elements from the between can cause changes
        int i = 0;
        contactNode* temp = head;
        while ( temp )
        {
            (*temp).index = i;
            i++;
            temp = (*temp).next;
        }
        cout<<"Indices reassigned."<<endl;
    }
    
    void addContact()
    {//a function to add a contact to the LL
        cout<<endl;
        cout<<"Enter new contact details (0 to go back)."<<endl;
        string name = "";
        string tempNo = "";
        unsigned long long phoneNo = 0;
        string email = "";

        cout<<"Enter name (compulsory): ";
        getline(cin, name, '\n');
        if ( name == "0" )
        {
            return;
        }

        cout<<"Enter phone number (compulsory): ";
        getline(cin, tempNo, '\n');
        if ( tempNo == "0" )
        {
            return;
        }
        phoneNo = strtoULL(tempNo);

        while (!phoneNo)
        {
            cout<<"Invalid phone number entered, try again: ";
            getline(cin, tempNo, '\n');
            phoneNo = strtoULL(tempNo);
        }

        cout<<"Enter email (~ to skip): ";
        getline(cin, email, '\n');
        if ( email == "0" )
        {
            return;
        }

        email = convertToLowercase(email);

        int newIndex = numberContacts;
        contactNode* newNode = new contactNode(newIndex, name, phoneNo, email);
        if ( tail == NULL )
        {
            head = newNode;
            tail = head; 
        }
        else
        {
            (*tail).next = newNode;
            tail = newNode;
        }
        numberContacts++;
        addition++;

        (*dictName).insertWord(name, newNode);
        (*dictNum).insertWord(tempNo, newNode);                                         //adding the contact details to the dictionary, as well as changing other properties of our phonebook object respectively

        cout<<endl<<"New contact created: "<<endl;
        display(newNode);
    }

    void deleteContact(int index)
    {//a function to delete a contact based on the index
        contactNode* temp = head;
        if ( (*temp).index == index )
        {
            cout<<"head deletion in progress..."<<endl;
            contactNode* next = (*temp).next;
            
            string tempNo = ULLtoStr((*temp).phoneNo);
            (*dictName).removeWord((*temp).name, temp);
            (*dictNum).removeWord(tempNo, temp);

            delete temp;
            head = next;

            numberContacts--;
            deletion++;
        
            indexReassign();
            return;
        }
        while ( temp )
        {
            contactNode* next = (*temp).next;
            if ( next != NULL && (*next).index == index )
            {
                cout<<"node found, deletion in progress..."<<endl;

                contactNode* nextNext = (*next).next;
                (*temp).next = nextNext;
                if ( nextNext == NULL )
                {
                    tail = temp;
                }
                else
                {
                    indexReassign();
                }

                numberContacts--;
                deletion++;
                
                string tempNo = ULLtoStr((*next).phoneNo);
                (*dictName).removeWord((*next).name, next);
                (*dictNum).removeWord(tempNo, next);

                delete next;
                break;
            }
            else if ( next == NULL )
            {
                cout<<"Node not found."<<endl;
            }

            temp = (*temp).next;
        }
    }

    void displayContact(int index)
    {//a function to disply contact information for an index
        contactNode* temp = head;
        while ( temp )
        {
            if ( (*temp).index == index )
            {
                cout<<"node found, displaying information..."<<endl;
                display(temp);
                return;
            }
            temp = (*temp).next;
        }
        cout<<"This index does not exist."<<endl;
    }

    void modifyContact(int index)
    {//a function to modify contact information for an index
        cout<<endl;
        contactNode* temp = head;
        while ( temp )
        {
            if  ( (*temp).index == index )
            {
                cout<<"Current information: "<<endl;
                display(temp);

                cout<<endl<<"Enter new information (~ to use previous information): "<<endl;
                string name = "";
                string tempNo = "";
                unsigned long long phoneNo = 0;
                string email = "";

                bool modified = false;

                cout<<"Enter name (compulsory): ";
        
                getline(cin, name, '\n');
                if ( name == "~" )
                {
                    name = (*temp).name;
                }
                else
                {
                    modified = true;
                }

                cout<<"Enter phoneNo (compulsory): ";
                getline(cin, tempNo, '\n');
                if ( tempNo == "~" )
                {
                    phoneNo = (*temp).phoneNo;
                }
                else
                {
                    phoneNo = strtoULL(tempNo);
                    
                    while (!phoneNo)
                    {
                        cout<<"invalid phone no. entered, try again: ";
                        getline(cin, tempNo, '\n');
                        phoneNo = strtoULL(tempNo);
                    }
                    modified = true;
                }
        

                cout<<"Enter email: ";
                getline(cin, email, '\n');
                if ( email == "~" )
                {
                    email = (*temp).email;
                }
                else
                {
                    email = convertToLowercase(email);
                    modified = true;
                }

                string tempNoRemove = ULLtoStr((*temp).phoneNo);
                (*dictName).removeWord((*temp).name, temp);
                (*dictNum).removeWord(tempNoRemove, temp);

                (*temp).name = name;
                (*temp).phoneNo = phoneNo;
                (*temp).email = email;

                (*dictName).insertWord(name, temp);
                (*dictNum).insertWord(tempNo, temp);

                cout<<"updated details: "<<endl;
                display(temp);
                
                if (modified)
                {
                    modification++;
                }

                return;
            }
            
            temp = (*temp).next;
        }

        cout<<"Contact with index not found."<<endl;
    }
};
