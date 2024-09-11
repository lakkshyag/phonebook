#include "contactNode.h"
#include "trieNode.h"
#include "phoneBook.h"
#include "userAccessProvidePage.cpp"
#include "contactImportPage.cpp"
#include "importedContacts.cpp"
using namespace std;

int commitChangesPage(phoneBook* userHost)
{//a function which prints all the changes made
    cout<<"You have made the following changes: "<<endl;
    cout<<"Number of contacts added: "<<(*userHost).addition<<endl;
    cout<<"Number of contacts modified: "<<(*userHost).modification<<endl;
    cout<<"Number of contacts deleted: "<<(*userHost).deletion<<endl;
    cout<<"Number of contacts imported: "<<(*userHost).imports<<endl<<endl;
    
    cout<<"Commit changes? (1 = yes; 0 = no) ";
    int opt;
    cin>>opt;
    cin.ignore(1, '\n');

    return opt;
}


int userContactPage(string username, string password)
{//main function for this page
    cout<<"___________________________________________________________________________________"<<endl<<endl;
    cout<<"Welcome "<<username<<endl;

    phoneBook* userHost = new phoneBook(username, password);
    contactNode* head = (*userHost).createContactFile();                                        //a function which reads the contacts from txt and creates a LL

    cout<<"You currently have "<<(*userHost).numberContacts<<" contact(s) saved."<<endl<<endl;

    int opt = 1;

    while ( opt )
    {
        cout<<"___________________________________________________________________________________"<<endl<<endl;
        cout<<"Enter: "<<endl;
        cout<<"0 to go log out."<<endl;
        cout<<"1 to display all contacts."<<endl;
        cout<<"2 to search for a contact."<<endl;
        cout<<"3 to add a new contact."<<endl;
        cout<<"4 to modify a contact."<<endl;
        cout<<"5 to delete a contact."<<endl;
        cout<<"6 to delete all contacts."<<endl;
        cout<<"7 to find out the number of contacts saved."<<endl;
        cout<<"8 to provide access of your contact list to other users."<<endl;
        cout<<"9 to import contacts from other users"<<endl;

        cin>>opt;
        cin.ignore(1, '\n');

        if ( opt == 0 )
        {
            break;
        }
        else if ( opt == 1 )
        {
            (*userHost).displayContactsAll();
        }
        else if ( opt == 2 )
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
                cout<<endl<<"Enter index to display: ";
                int index;
                cin>>index;
                cin.ignore(1, '\n');
                (*userHost).displayContact(index);
            }
            else if ( subOpt == 2 )
            {                
                int index = searchByName(userHost);
                (*userHost).displayContact(index);
            }
            else if ( subOpt == 3 )
            {
                int index = searchByNumber(userHost);
                (*userHost).displayContact(index);             
            }
        }
        else if ( opt == 3 )
        {
            (*userHost).addContact();
        }
        else if ( opt == 4 )
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
                cout<<"Enter index to modify: ";
                int index;
                cin>>index;
                cin.ignore(1, '\n');
                (*userHost).modifyContact(index);
            }
            else if ( subOpt == 2 )
            {                
                int index = searchByName(userHost);
                (*userHost).modifyContact(index);
            }
            else if ( subOpt == 3 )
            {
                int index = searchByNumber(userHost);
                (*userHost).modifyContact(index);             
            }
        }
        else if ( opt == 5 )
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
                cout<<"Enter index to delete: ";
                int index;
                cin>>index;
                cin.ignore(1, '\n');
                (*userHost).deleteContact(index);
            }
            else if ( subOpt == 2 )
            {                
                int index = searchByName(userHost);
                (*userHost).deleteContact(index);
            }
            else if ( subOpt == 3 )
            {
                int index = searchByNumber(userHost);
                (*userHost).deleteContact(index);             
            }
        }
        else if ( opt == 6 )
        {
            cout<<endl;
            cout<<"Enter password to confirm deletion of all contacts: ";
            string input;
            cin>>input;
            cin.ignore(1, '\n');
            if ( input == password )
            {
                (*userHost).deleteContactsAll();
            }
            else
            {
                cout<<"Credentials mismatched, failed to delete all contacts"<<endl;
            }
        }
        else if ( opt == 7 )
        {
            cout<<(*userHost).numberContacts<<" contacts currently saved."<<endl;
        }
        else if ( opt == 8 )
        {
            opt = userAccessProvidePage(username);
        }
        else if ( opt == 9 )
        {
            canAccessUser cAU = contactImportPage(username);
            if ( cAU.accessSuccess )
            {
                cout<<"heading to "<<cAU.accessUsername<<"'s contact list..."<<endl;
                opt = importedContactsPage(username, userHost, cAU.accessUsername);
            }
            else
            {
                cout<<"access failed, staying on this page..."<<endl;
            }
        }
        else
        {
            //nothing
        }
    }

    cout<<"presave..."<<endl;

    opt = commitChangesPage(userHost);

    if (opt) 
    {
        (*userHost).saveContactFile();
        cout<<"saved..."<<endl;
    }
    else
    {

        cout<<"changes not saved..."<<endl;
    }
    
    delete userHost;
    return 0;
}