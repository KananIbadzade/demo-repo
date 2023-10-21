// Kanan Ibadzade - CIS22C - HW1

#include <iostream>
#include <fstream>
using namespace std;



class Student
{   

private:
    int sid; 
    string sname; 

public:
    Student() { 
        sid = 0; sname.clear();
    };
    Student(const Student& os) { 
        sid = os.sid;  sname = os.sname;
    };
    Student(int id, string name) { 
        sid = id; sname = name;
    };
    ~Student() { 
        sname.clear();
    };


    string getName() { return (sname); };
    int getId() { return (sid); };
};



// 2. *********** Homework 1.  insert the code for the StudentEsa into the StudentEsa interface below *******


class StudentEsa
{
private:  // Define whatever additional private variables you need
    Student** sap; // Underlying pointer to array of Student pointers, which you will allocate
    Student** tp;  // Optionally used in realloc
    int cnum; // Number of students currently in array
    int cmz; // Current Max size of the array (may need to be expanded)

public: // Publically supported methods YOU NEED TO IMPLEMENT.  Replace each declaration with complete code

    // ****************  Constructors / Destructor

    //  creates a new, larger-than-before array of student pointers and assigns it to tp.
    void realloc() {
        delete[] tp;
        tp = new Student * [cmz * 2]
        cmz *= 2;
    };

    StudentEsa(int ms)        //  1. Normal Constructor.  Create ESA of this size.  May need to be resized later
    {
        sap = new Student * [ms];
        cmz = ms;
        cnum = 0;
        tp = NULL;
    };

   
    StudentEsa()                    // 2. Default Constructor <Comment out, or use some predefined size, and EXPLAIN YOUR CHOICE>
    {
        sap = nullptr;
        tp = nullptr;
        cnum = 0;
        cmz = 0;
    };


    StudentEsa(StudentEsa& sep)   // 3. Copy Constructor  (what gets copied and what does not?)
    {
        cnum = sep.cnum;
        cmz = sep.cmz;
        tp = nullptr;
        sap = new Student * [cmz];

        for (int i = 0; i < cmz; i++) {   //copies student pointer
            sap[i] = new Student(*sep.sap[i]);
        }
    };


    ~StudentEsa()           // 4. Default destructor  (what gets freed up and what does not?)
    {
        delete[] sap;
    };

    // ******************* Remaining public functions of the Extended Student Array

        // 1. Return the number of Students in the Collection
    int getSize()
    {
        return cnum;
    };


    // 2. GET:  Get and return the ptr to the Student at the specified Array Index
    //          Error if the supplied index >= number of entries.  Return a NULL ptr
    Student* get(int index)
    {
        if ((index < 0) || (index >= cnum)) {
            return (NULL);
        }
        else {
            return (sap[index]);   // return student ptr at that index in the array
        } 
    }; 



    // 3. SET: Replace whatever is at the supplied index with the new Student ptr. Return index # on success
    // Error if the supplied index >= number of entries.  Return -1
    // <DO NOT Destruct the Student whose ptr is at the supplied index>
    // 
    int set(Student* sep, int index)
    {
        if (index < 0 || index >= cnum) return -1; //Return invalid index
        
        sap[index] = sep; //reassigns student at given index with given student
        return index;
    };

    // 4. INSERT: Insert Student ptr at the supplied index, by first "pushing back" every subsequent element
    //    Error if the supplied index > number of entries.  Return -1
    //    OK if supplied index == number of entries (equivalent to an append)
    //    Note:  ESA size increases which may force a reallocation of the array.  Return index # on success
    int insert(Student* nsp, int index)
    {
        if (index < 0 || index >= cmz) return -1;

        // Reallocs and pushes back if number of students reaches size of array
        if (cnum == cmz) {
            realloc(); 

            for (int i = 0; i < index; i++) {
                tp[i] = sap[i]; // sap is copied to tp
            }

            tp[index] = nsp; // adding new Student pointer at the given index

            for (int i = index; i < cnum; i++) {
                tp[i + 1] = sap[i];
            }
            sap = tp;
            tp = NULL;
            cnum++;

            return index;
        }

        // placing a student in an index while pushing back all following  elements
        Student* tempStudent = nsp;
        for (int i = index; i < cnum; i++) {
            Student* tempPtr = sap[i];
            sap[i] = tempStudent;
            tempStudent = tempPtr;
        }
        sap[cnum] = tempStudent; // checks that last element gets updated 

        cnum++; // number of students is increased by one
        return index;
    };

    // 5. REMOVE: Delete specified array element by "pulling forward" every subsequent element
    //    Error if the supplied index >= number of entries.  Return -1.  Else decrement ESA size and return it
    //    Note: <DO NOT Destruct the Student whose ptr is at the supplied index>
    int remove(int index)
    {
        if (index >= cmz || index < 0) return -1; //returns error for invalid index
        Student* nsp = sap[index];    
        
        for (int i = index + 1; i < cnum; i++) {   // overwriting the Student at the specified index
            sap[i - 1] = sap[i];
        }
        cnum -= 1; // after removing a student
        return index;
    };


    // 6. APPEND:  Append the ptr to the supplied Student to back of the ESA, bump ESA size
    //    Return index # of new entry on success, -1 on failure
    //    Note:  This may force a reallocation of the array.
    int append(Student* nsp)
    {
        return insert(nsp, cnum);
    };


    // 7. PREPEND: Prepend the ptr to the supplied Student to the front of the ESA, increment ESA size
    //    Return 0 on success, -1 on failure
    //    Note:  This may force a reallocation of the array.  
    int prepend(Student* nsp)
    {
        return insert(nsp, 0);
    };
};

// **************************** End class StudentEsa ******************************




// 3.  ***********************  Start of StudentEsa Test Program Code  ************

int main()
{ 
    string infileName; 
    string outfileName; 
    FILE** input = NULL;  

    int ssize; 
    int nops; 

    char command; 

    int index; 
    int num;  
    string name;  

    StudentEsa* esa = NULL;   
    Student sc; 

    int x = 0; 

    cout << "Use input file:  ";
    cin >> infileName; 
    cout << "Using input file " << infileName << endl << endl;

    
    std::ifstream inp;
    inp.open(infileName.c_str());
    if (!inp) {
        cerr << "Error: file " << infileName.c_str() << "  could not be opened" << endl;
        exit(1);
    }

    // First line is array size and # commands to add, every subsequent line is one of:
    //  Append:  A / -1 / StudentID / Student Name
    //  Insert:  I / Index to Insert / StudentID / Student Name
    //  Remove:  R / Index to Remove / -1 / XXXX
    //  Store:   S / Index to Store (overwrite) /  new StudentID / new Student Name

    inp >> ssize >> nops;
    cout << "Read Array size " << ssize << "  Read # commands " << nops << endl;

    Student* stud;    
    esa = new StudentEsa(ssize);  


    for (int i = 0; i < nops; i++) { 
        
        inp >> command;
        inp >> index;
        inp >> num;
        inp >> name;
        cout << "Command: " << command << "  " << index << "  " << num << "  " << name << endl;

        // 
        switch (command)
        { 
        case 'A':  
            stud = new Student(num, name);
            esa->append(stud);
            break;
        case 'I':  
            stud = new Student(num, name);
            esa->insert(stud, index);
            break;
        case 'R':  
            esa->remove(index);
            break;
        case 'S': 
            stud = new Student(num, name);
            esa->set(stud, index);
            break;
        default:
            cout << "Illegal Command:  " << command << endl;
        }

    }
    
    cout << "-------" << endl << endl;

    x = esa->getSize(); 
    for (int i = 0; i < x; i++) {  
        stud = esa->get(i); 
        cout << i << "  Student: ID = " << stud->getId() << "  Name = " << stud->getName() << endl;
    }
    return (0);
}

