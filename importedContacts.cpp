//using namespace std;
int searchByName(phoneBook* userHost)
{//a function to use our name dictionary to perform auto complete searches based on name
    cout<<endl<<"Enter name to search: ";
    string name = "";
    getline(cin, name, '\n');

    (*(*userHost).dictName).autoCompleter(name);

    cout<<"Choose an index from the following: ";
    int index;
    cin>>index;
    cin.ignore(1, '\n');

    return index;
}

int searchByNumber(phoneBook* userHost)
{//similar thing but just for phone numbers instead of names
    cout<<"Enter number to search: ";
    string tempNo = "";
    getline(cin, tempNo, '\n');

    (*(*userHost).dictNum).autoCompleter(tempNo);

    cout<<"Choose an index from the following: ";
    int index;
    cin>>index;
    cin.ignore(1, '\n');

    return index;
}


void importContact(phoneBook* const &userHost, phoneBook* userAccess, int index)
{//function to import a contact from access' contact list to host's contact list
    contactNode* temp = (*userAccess).head;
    while ( temp )
    {
        if ( (*temp).index == index )
        {
            cout<<"node found, importing..."<<endl;
            string name = (*temp).name;
            unsigned long long phoneNo = (*temp).phoneNo;
            string email = (*temp).email;

            int newIndex = (*userHost).numberContacts;

            contactNode* newNode = new contactNode(newIndex, name, phoneNo, email);
            if ( (*userHost).tail == NULL )
            {
                (*userHost).head = newNode;
                (*userHost).tail = (*userHost).head; 
            }
            else
            {
                (*(*userHost).tail).next = newNode;
                (*userHost).tail = newNode;
            }
            (*userHost).numberContacts++;
            (*userHost).imports++;

            cout<<endl<<"New contact imported: "<<endl;
            (*userHost).display(newNode);
            
            string tempNo = ULLtoStr(phoneNo);
            
            (*(*userHost).dictName).insertWord(name, newNode);
            (*(*userHost).dictNum).insertWord(tempNo, newNode);

            return;
        }
        temp = (*temp).next;
    }
    cout<<"this index does not exist..."<<endl;
}

void importContactsAll(phoneBook* const &userHost, phoneBook* userAccess)
{//function to import all of access' contacts to host
    cout<<"importing all contacts..."<<endl;
    contactNode* temp = (*userAccess).head;

    while ( temp )
    {
        string name = (*temp).name;
        unsigned long long phoneNo = (*temp).phoneNo;
        string email = (*temp).email;

        int newIndex = (*userHost).numberContacts;

        contactNode* newNode = new contactNode(newIndex, name, phoneNo, email);
        if ( (*userHost).tail == NULL )
        {
            (*userHost).head = newNode;
            (*userHost).tail = (*userHost).head; 
        }
        else
        {
            (*(*userHost).tail).next = newNode;
            (*userHost).tail = newNode;
        }
        
        (*userHost).numberContacts++;
        (*userHost).imports++;

        string tempNo = ULLtoStr(phoneNo);
            
        (*(*userHost).dictName).insertWord(name, newNode);
        (*(*userHost).dictNum).insertWord(tempNo, newNode);

        temp = (*temp).next;
    }

    cout<<"All contacts imported."<<endl;
}

int importedContactsPage(string usernameHost, phoneBook* &userHost, string usernameAccess)
{//main function for this page
    cout<<endl<<"___________________________________________________________________________________"<<endl<<endl;
    cout<<"Viewing contacts from "<<usernameAccess<<endl;

    phoneBook* userAccess = new phoneBook(usernameAccess, "");
    contactNode* headAccess = (*userAccess).createContactFile();

    cout<<usernameAccess<<" currently has "<<(*userAccess).numberContacts<<" contacts saved."<<endl;

    int opt = 1;

    while ( opt )
    {
        cout<<"Enter: "<<endl;
        cout<<"0 to exit import page."<<endl;
        cout<<"1 to search for a contact."<<endl;
        cout<<"2 to display all contacts of this user's phonebook."<<endl;
        cout<<"3 to import a contact to your phonebook."<<endl;
        cout<<"4 to import all contacts from this user's phonebook to your phonebook"<<endl;

        cin>>opt;
        cin.ignore(1, '\n');

        if ( opt == 0 )
        {
            break;
        }
        else if ( opt == 1 )
        {
            cout<<endl<<"Enter: "<<endl;
            cout<<"0 to go back."<<endl;
            cout<<"1 to search by index."<<endl;
            cout<<"2 to search by name."<<endl;
            cout<<"3 to search by phone number."<<endl;

            int subOpt;
            cin>>subOpt;
            cin.ignore(1, '\n');
            if ( subOpt == 0 )
            {
                // do nothing
            }
            else if ( subOpt == 1 )
            {
                cout<<"Enter index to display: ";
                int index;
                cin>>index;
                cin.ignore(1, '\n');
                (*userAccess).displayContact(index);
            }
            else if ( subOpt == 2 )
            {                
                int index = searchByName(userAccess);
                (*userAccess).displayContact(index);
            }
            else if ( subOpt == 3 )
            {
                int index = searchByNumber(userAccess);
                (*userAccess).displayContact(index);             
            }
        }
        else if ( opt == 2 )
        {
            (*userAccess).displayContactsAll();
        }
        else if ( opt == 3 )
        {
            cout<<endl<<"Enter: "<<endl;
            cout<<"0 to go back."<<endl;
            cout<<"1 to search by index."<<endl;
            cout<<"2 to search by name."<<endl;
            cout<<"3 to search by phone number."<<endl;
            
            int subOpt;
            cin>>subOpt;
            cin.ignore(1, '\n');
            if ( subOpt == 0 )
            {
                // do nothing
            }
            else if ( subOpt == 1 )
            {
                cout<<"Enter index to import: ";
                int index;
                cin>>index;
                cin.ignore(1, '\n');
                importContact(userHost, userAccess, index);
            }
            else if ( subOpt == 2 )
            {                
                int index = searchByName(userAccess);
                importContact(userHost, userAccess, index);
            }
            else if ( subOpt == 3 )
            {
                int index = searchByNumber(userAccess);
                importContact(userHost, userAccess, index);      
            }
        }        
        else if ( opt == 4 )
        {
            importContactsAll(userHost, userAccess);
        }
        else
        {
            //nothing
        }
    }

    delete userAccess;
    cout<<"returning to contacts page..."<<endl;

    return 1;
}