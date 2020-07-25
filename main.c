#include "cabrental.h"
#include <math.h>
#include <conio.h>
#include <stdlib.h>
#define null NULL
#define PRICE_PER_KM 10

void AddDriver (Driver** dhead, Status** shead)
{
    Driver* dnew = (Driver*)malloc(sizeof(Driver));
    Status* snew = (Status*)malloc(sizeof(Status));
    GetDriverDetails(dnew, snew);

    Driver* dptr = *dhead;
    Status* sptr = *shead;
    
    if(dptr == null)
    {
        if(sptr == null)
        {
            *dhead = dnew;
            *shead = snew;
            return;
        }
        else
        {
            printf("\nERROR BIG ERROR IN AddDriver\n");
            return;
        }
    }
    
    while(dptr->next != null)
    { // MAKE SURE NO OF STATUS NODES = NO OF DRIVER NODES
        dptr = dptr->next;
        sptr = sptr->next;
    }

    dptr->next = dnew;
    dnew->prev = dptr;
    dnew->next = null;

    sptr->next = snew;
    snew->prev = sptr;
    snew->next = null;
    return;
}

void GetDriverDetails(Driver* d, Status* s)
{
    ull id;
    printf("Enter driver ID: ");
    scanf("%llu", &id);
    d->d_ID = id;
    printf("Enter driver name: ");
    scanf("%s", &d->name);
    printf("Enter type of vehicle [auto, bike, car]: ");
    scanf("%s", &d->vehicletype);
    
    s->d_ID = id;
    printf("Enter x coordinate: ");
    scanf("%f", &s->location.x);
    printf("Enter y coordinate: ");
    scanf("%f", &s->location.y);

    printf("Enter status [1 for booked, 0 for free]: ");
    scanf("%llu", &s->status);

    d->next = null;
    s->next = null;
}

ull AssignDriver (Status* shead)
{
    Status* sptr = shead;
    if(sptr == null)
    {
        printf("\nEMpty FROM AssignDriver\n");
    }
    
    while (sptr!= null)
    {
        if(sptr->status == 0)
        {
            sptr->status = 1;
            return sptr->d_ID;
        }
        sptr = sptr->next;
    }
    return 0;
}

void NewBooking(Passenger** phead, History** histhead, Status* shead, Driver* dhead)
{/*
    1.  Get passenger ID
    2.  Assign driver
    3.  if passenger ID doesn't exist in the (sorted) linked list
            add passenger to Passenger list 
        else (if passenger exists)
            update pickup and drop locations of 
             existing passenger to the new locations 
    4.  if driver-passenger pair exists in History linked list:
            update frequency
            increment distance_travelled
        else           
            add pair to list    */        
    ull pid;
    int passengerexists = 0;
    printf("Enter passenger ID: ");
    scanf("%llu", &pid);
    ull assigned_id = AssignDriver(shead);

    Passenger* pptr = *phead;
    History* hptr = *histhead;
    Driver* dptr = dhead;
    Passenger* pnew;
    Passenger* existingPassenger;

    if(*phead == null)
    {
        pnew = AddPassenger(phead, pid);
        AddHistory(histhead, assigned_id, pnew);
    }
    else
    {
        passengerexists = 0;
        while(pptr != null && passengerexists == 0)
        {
            if(pptr->p_ID == pid)
            {
                printf("Passenger found! Update details: \n");
                passengerexists = 1;
                
                printf("Enter passenger's current location\n\tx coordinate: ");
                scanf("%f", &pptr->p_location.x);
                printf("\ty coordinate: ");
                scanf("%f", &pptr->p_location.y);

                printf("Enter drop location\n\tx coordinate: ");
                scanf("%f", &pptr->drop_location.x);
                printf("\ty coordinate: ");
                scanf("%f", &pptr->drop_location.y);
                existingPassenger = pptr;
            }
            else
                pptr = pptr->next;
        }

        if(passengerexists == 0) // means end of list reached
        {
            printf("NEW ACCOUNT:\n");
            pnew = AddPassenger(phead, pid);
            AddHistory(histhead, assigned_id, pnew);   
        }
        
        else
        {
            hptr = *histhead;
            // (if passenger exists then history of that passenger obviously exists.)
            while(hptr != null)
            {
                if(hptr->p_ID == pid)
                {
                    if(hptr->d_ID == assigned_id) 
                    {
                        printf("Inside func. \n");
                        hptr->frequency = hptr->frequency + 1;
                        float d = CalcDistance (existingPassenger->p_location, existingPassenger->drop_location);
                        hptr->distance_travelled += d;
                        printf("Distance increased by %f\n", d);
                    }
                }
                hptr = hptr->next;
            }
        }
    }

    while(dptr != null)
    {
        if(dptr->d_ID == assigned_id)
        {
            printf("\nBOOKING CONFIRMED! Driver %s is on their way to pick you up in a %s!", dptr->name, dptr->vehicletype);
            printf("\n\nDo you want to finish trip and free this driver? [y/n]: "); 
            char s[5];
            scanf("%s", &s);
            if (s[0] == 'y' || s[0] == 'Y')
                FinishTrip(dptr->d_ID, shead);
            return;
        }
        dptr = dptr->next;
    }
}

void FinishTrip (ull id, Status* shead)
{
    Status* ptr = shead;
    while(ptr != null)
    {
        if(ptr->d_ID == id)
        {
            ptr->status = 0;
            return;
        }
        ptr = ptr->next;
    }
}

void AddHistory(History** histhead, ull driverID, Passenger* pnode)
// adds history node at end of History
{
    printf("Making history... ");
    History* hnew = (History*)malloc(sizeof(History));
    History* hptr = *histhead;

    hnew->frequency = 1;
    hnew->d_ID = driverID;
    hnew->p_ID = pnode->p_ID;
    hnew->p_mobile_no = pnode->p_mobile_no;
    hnew->distance_travelled = CalcDistance(pnode->p_location, pnode->drop_location);

    if(*histhead == null)
    {
        *histhead = hnew;
        return;
    }

    while(hptr->next != null)
        hptr = hptr->next;

    hptr->next = hnew;
    hnew->next = null;
    hnew->prev = hptr;
    printf("history made!\n");
}

Passenger* AddPassenger (Passenger** head, ull pid)
// adds at end of passenger list
{
    ull id;
    Passenger* pnew = (Passenger*)malloc(sizeof(Passenger));
    Passenger* ptr = *head;
    GetPassengerDetails(pnew, pid);

    if(*head == null)
    {
        *head = pnew;
        return *head;
    }
    
    while (ptr->next != null)
        ptr = ptr->next;

    ptr->next = pnew;
    pnew->prev = ptr;
    pnew->next = null;
    return pnew;
}

void GetPassengerDetails(Passenger* p, ull pid)
{
    p->p_ID = pid;
    printf("Enter passenger name: ");
    scanf("%s", &p->name);
    printf("Enter passenger mobile number: ");
    scanf("%llu", &p->p_mobile_no);

    printf("Enter passenger's current location\n\tx coordinate: ");
    scanf("%f", &p->p_location.x);
    printf("\ty coordinate: ");
    scanf("%f", &p->p_location.y);

    printf("Enter drop location\n\tx coordinate: ");
    scanf("%f", &p->drop_location.x);
    printf("\ty coordinate: ");
    scanf("%f", &p->drop_location.y);

    p->next = null;
    p->prev = null;
}

float CalcDistance (Location start, Location end)
{
    return sqrt(((start.x - end.x)*(start.x - end.x)) 
                + ((start.y - end.y)*(start.y - end.y)));
}

Status* CalcNearby (Passenger* pnode, Status* shead)
{
    Status* slist = null, *prevnode = null;
    Status* sptr = shead;

    if (shead == null)
        return shead;
    if(shead->next == null)
    {
        if(shead->status == 0 && CalcDistance(shead->location, pnode->p_location) <= 5.0)
            return shead;

        else return null;
    }

    while(sptr != null)
    {
        if(sptr->status == 0 && CalcDistance(sptr->location, pnode->p_location) <= 5.0)
        {
            Status* snew = (Status*)malloc(sizeof(Status));

            snew->d_ID = sptr->d_ID;
            snew->location = sptr->location;
            snew->status = 0;

            if(prevnode != null)
                prevnode->next = snew;
            snew->prev = prevnode;
            snew->next = null;
            prevnode = snew;
            
            if(slist == null)
            {
                // if free, nearby and no node created yet
                slist = snew;
                prevnode = slist;
            }
        }
        sptr = sptr->next;
    }
    return slist;
}

void UpdateDriver(Driver* dhead,  Status* shead)
{
    // D R I V E R
    FILE* fdriv = fopen("data/driver.txt", "w");
    if(fdriv == null)
        printf("driver.txt failed to open.");
    else
    {
        if(dhead == null)
            printf("DHEAD IS NULL");
        while(dhead != null)
        {
            fprintf(fdriv, "%llu %s %s\n", dhead->d_ID, dhead->name, dhead->vehicletype);
            dhead = dhead->next;
        }
    }
    fclose(fdriv);

}

void UpdateStatus (Status* shead)
{
    // S T A T U S
    FILE* fstat = fopen("data/status.txt", "w");
    if(fstat == null)
        printf("status.txt failed to open.");
    else
    {
        if(shead == null)
            printf("SHEAD IS NULL");
        while(shead != null)
        {
            fprintf(fstat, "%llu %.1f %.1f %llu\n", shead->d_ID, shead->location.x, shead->location.y, shead->status);
            
            shead = shead->next;
        }
    }
    fclose(fstat);
}

void UpdateHistoryPassenger(Passenger* phead, History* hhead)
{
    // P A S S E N G E R
    FILE* fpass = fopen("data/passenger.txt", "w");
    if(fpass == null)
        printf("passenger.txt failed to open.");
    else
    {
        if(phead == null)
            printf("PHEAD IS NULL");
        else while(phead != null)
        {
            fprintf(fpass, "%llu %llu %.1f %.1f %.1f %.1f %s\n", phead->p_ID, phead->p_mobile_no, phead->p_location.x, phead->p_location.y, phead->drop_location.x, 
                phead->drop_location.y, phead->name);

            phead = phead->next;
        }
    }
    fclose(fpass);

    // H I S T O R Y
    FILE* fhist = fopen("data/history.txt", "w");
    if(fhist == null)
        printf("history.txt failed to open.");
    else
    {
        if(hhead == null)
            printf("HHEAD IS NULL");
        while(hhead != null)
        {
            fprintf(fhist, "%llu %llu %llu %llu %.1f\n", hhead->d_ID, hhead->p_ID,hhead->p_mobile_no, hhead->frequency, hhead->distance_travelled);
            
            hhead = hhead->next;
        }
    }
    fclose(fhist);
}

Driver* ReadDriver()
{
    Driver *dhead = null;
    Driver *dptr = dhead;
    Driver *prevnode = null;

    ull d_ID;
    char name[SIZE], vehicletype[SIZE];

    FILE* fp = fopen("data/driver.txt", "r");
    if(fp == null)
        printf("driver.txt failed to open.");
    else
    {
        while (!feof(fp))
        {
            fscanf(fp, "%llu %s %s\n", &d_ID, &name, &vehicletype);
            Driver* newnode = (Driver*)malloc(sizeof(Driver));

            newnode->d_ID = d_ID;
            strcpy(newnode->name, name);
            strcpy(newnode->vehicletype, vehicletype);

            if(prevnode != null)
                prevnode->next = newnode;
            newnode->prev = prevnode;    
            newnode->next = null;
            prevnode = newnode;

            if(dhead == null)
            {
                dhead = newnode;
                prevnode = dhead;
            }
        }
    }
    fclose(fp);
    return dhead;
}

Passenger* ReadPassenger()
{
    Passenger *head = null;
    Passenger *dptr = head;
    Passenger *prevnode = null, *newnode = null;

    ull p_ID, p_mobile_no;
    Location p_location, drop_location;
    char name[SIZE];

    FILE* fp = fopen("data/passenger.txt", "r");
    if(fp == null)
        printf("passenger.txt failed to open.");
    else while (!feof(fp))
    {
        fscanf(fp, "%llu %llu %f %f %f %f %s\n", &p_ID, &p_mobile_no, &p_location.x, &p_location.y, &drop_location.x, &drop_location.y, &name);

        newnode = (Passenger*)malloc(sizeof(Passenger));

        newnode->p_ID = p_ID;
        newnode->p_mobile_no = p_mobile_no;
        newnode->p_location = p_location;
        newnode->drop_location = drop_location;
        strcpy(newnode->name, name);

        if(prevnode != null)
            prevnode->next = newnode;
        newnode->prev = prevnode;    
        newnode->next = null;
        prevnode = newnode;

        if(head == null)
        {
            head = newnode;
            prevnode = head;
        }
    }
    fclose(fp);
    return head;
}

History* ReadHistory()
{
    History *head = null;
    History *dptr = head;
    History *prevnode = null;

    ull d_ID, p_ID, p_mobile_no, frequency;
    float distance_travelled;

    FILE* fp = fopen("data/history.txt", "r");
    if(fp == null)
        printf("history.txt failed to open.");
    else
    {
        while (!feof(fp))
        {
            fscanf(fp, "%llu %llu %llu %llu %f\n", &d_ID, &p_ID, &p_mobile_no, &frequency, &distance_travelled);

            History* newnode = (History*)malloc(sizeof(History));
            
            newnode->d_ID = d_ID;
            newnode->p_ID = p_ID;
            newnode->p_mobile_no = p_mobile_no;
            newnode->frequency = frequency;
            newnode->distance_travelled = distance_travelled;

            if(prevnode != null)
                prevnode->next = newnode;
            newnode->prev = prevnode;    
            newnode->next = null;
            prevnode = newnode;

            if(head == null)
            {
                head = newnode;
                prevnode = head;
            }
        }
    }
    fclose(fp);
    return head;
}

Status* ReadStatus()
{
    Status *head = null;
    Status *dptr = head;
    Status *prevnode = null;

    ull d_ID, status;
    float locx, locy;

    FILE* fp = fopen("data/status.txt", "r");
    if(fp == null)
        printf("status.txt failed to open.");
    else
    {
        while(!feof(fp))
        {
            fscanf(fp, "%llu %f %f %llu\n", &d_ID, &locx, &locy, &status);

            Status* newnode = (Status*)malloc(sizeof(Status));
            newnode->d_ID = d_ID;
            newnode->status = status;
            newnode->location.x = locx;
            newnode->location.y = locy;

            if(prevnode != null)
                prevnode->next = newnode;
            newnode->prev = prevnode;    
            newnode->next = null;
            prevnode = newnode;

            if(head == null)
            {
                head = newnode;
                prevnode = head;
            }
        }
    }
    fclose(fp);
    return head;
}

void MostFrequentPair(History* head, Driver* dhead, Passenger* phead)
{
    ull max = 0, maxpid = 0, maxdid = 0;
    char dname[SIZE], pname[SIZE];
    if(head == null)
        printf("No records found!");

    else while (head != null)
    {
        if(head->frequency > max)
        {
            max = head->frequency;
            maxdid = head->d_ID;
            maxpid = head->p_ID;
        }
        head = head->next;
    }
    
    while(dhead != null)
    {
        if(dhead->d_ID == maxdid)
        {
            // printf("DRIVER NAME COPIED");
            strcpy(dname, dhead->name);
        }
        dhead = dhead->next;
    }

    while(phead != null)
    {
        if(phead->p_ID == maxpid)
        {
            // printf("PASSENGER NAME COPIED");
            strcpy(pname, phead->name);
        }
        phead = phead->next;
    }

    printf("MOST FREQUENT PAIR: passenger #%llu (%s) and driver #%llu (%s)  (%llu times)\n", maxpid, pname, maxdid, dname, max);
}

void showDriver()
{
    ull d_ID;
    char name[SIZE], vehicletype[SIZE];

    printf("************** DRIVER DETAILS **************\n");

    FILE* fp = fopen("data/driver.txt", "r");
    if(fp == null)
        printf("driver.txt failed to open.");
    else
    {

        printf("d_ID\t\tname\t\tvehicleType\n");
        while (!feof(fp))
        {
            fscanf(fp, "%llu %s %s\n", &d_ID, &name, &vehicletype);
            printf("%llu\t\t%s\t\t%s\n", d_ID, name, vehicletype);
        }
    }
    fclose(fp);
}

void showPassenger()
{
    ull p_ID, p_mobile_no;
    Location p_location, drop_location;
    char name[SIZE];

    printf("************** PASSENGER DETAILS **************\n");

    FILE* fp = fopen("data/passenger.txt", "r");
    if(fp == null)
        printf("passenger.txt failed to open.");
    else
    {
        printf("p_ID\tmobile\t\tpickup\t\tdrop\t\tname\n");
        while (!feof(fp))
        {
            fscanf(fp, "%llu %llu %f %f %f %f %s\n", &p_ID, &p_mobile_no, &p_location.x, &p_location.y, &drop_location.x, &drop_location.y, &name);

            printf("%llu\t%llu\t\t(%.1f, %.1f)\t(%.1f, %.1f)\t%s\n", p_ID, p_mobile_no, p_location.x, p_location.y, drop_location.x, drop_location.y, name);
        }
    }
    fclose(fp);
}

void showHistory()
{
    ull d_ID, p_ID, p_mobile_no, frequency;
    float distance_travelled;

    printf("************** HISTORY **************\n");

    FILE* fp = fopen("data/history.txt", "r");
    if(fp == null)
        printf("history.txt failed to open.");
    else
    {
        printf("d_ID\tp_ID\tmobile\tfreq\tdistance\n");
        while (!feof(fp))
        {
            fscanf(fp, "%llu %llu %llu %llu %f\n", &d_ID, &p_ID, &p_mobile_no, &frequency, &distance_travelled);

            printf("%llu\t%llu\t%llu\t%llu\t%.1f\n", d_ID, p_ID, p_mobile_no, frequency, distance_travelled);
        }
    }
    fclose(fp);
}

void showStatus()
{
    ull d_ID;
    Location location;  
    ull status;

    printf("************** DRIVER STATUS **************\n");

    FILE* fp = fopen("data/status.txt", "r");
    if(fp == null)
        printf("status.txt failed to open.");
    else
    {
        printf("d_ID\tlocation\t\tstatus\n");
        while (!feof(fp))
        {
            fscanf(fp, "%llu %f %f %llu\n", &d_ID, &location.x, &location.y, &status);

            printf("%llu\t(%.1f, %.1f)\t\t%llu\n",d_ID, location.x, location.y, status);
        }
    }
    fclose(fp);
}

void showAll()
{
    showDriver();       printf("\n");
    showPassenger();    printf("\n");
    showHistory();      printf("\n");
    showStatus();       printf("\n");
}

float CalculateCost (Driver* dptr, History* histhead)
{
    ull id = dptr->d_ID;
    float dist = 0;
    History* hptr = histhead;
    
    while(hptr != null)
    {
        if(hptr->d_ID == id)
            dist += hptr->distance_travelled;
        
        hptr = hptr->next;
    }
    return dist*PRICE_PER_KM;
}

float SuccessfulDrivers (Driver* dhead, History* histhead)
{
    Driver* dptr = dhead;
    History* hptr = histhead;
    float cost;

    typedef struct earnings{
        float money;
        ull ID;
        char name[SIZE];
    }earnings;
    earnings e[3];

    if(dptr == null)
    {
        if(hptr == null)
            return 0;
        else
        {
            // printf("\n\np IN CalculateCost\n\n");
            return -1;
        }
    }

    int c = 0;
    while(dptr && hptr && c<3) 
    {    
        e[c].ID = dptr->d_ID;
        strcpy(e[c].name, dptr->name);
        e[c].money += CalculateCost(dptr, histhead);
        
        // printf("c = %d, money this loop = %f\n, id = %llu, name = %s\n", c, e[c].money, e[c].ID, e[c].name);
        c++;
        dptr = dptr->next;
        // hptr = hptr->next;
    }

    int leastindex = 0;
    if(c > 0)
        printf("DRIVER EARNINGS:\nID\tName\tEarnings\n");

    if(c == 3)
    {
        // printf("\nC IS 3\n");
        leastindex = _leastIndex (e[0].money, e[1].money, e[2].money);
        while(dptr != null && hptr != null)
        {
            if((cost = CalculateCost(dptr, histhead)) > e[leastindex].money)
            {
                printf("\nCOST = %f\n", cost);
                e[leastindex].money = cost;
                e[leastindex].ID = dptr->d_ID;
                strcpy(e[leastindex].name, dptr->name);
                leastindex = _leastIndex(e[0].money, e[1].money, e[2].money);
            }
            dptr = dptr->next;
        }
        int i = 2;
        while(i>=0)
        {
            printf("%llu\t%s\t%.1f\n", e[i].ID, e[i].name, e[i].money);
            i--;
        }
    }
    else if (c == 2)
    {
        int i = e[1].money > e[0].money ? 1 : 0;
        printf("%llu\t%s\t%.1f\n", e[i].ID, e[i].name, e[i].money);
        i = (i == 1) ? 0 : 1;
        printf("%llu\t%s\t%.1f\n", e[i].ID, e[i].name, e[i].money);
    }

    else if (c == 1)
        printf("%llu\t%s\t%.1f\n", e[0].ID, e[0].name, e[0].money);

    else
        printf("No drivers!");
}

int _leastIndex (float a, float b, float c)
{
    if(a < b)   return (a < c) ? 0 : 2;
    else        return (b < c) ? 1 : 2;
}

int main()
{
    char str[5];
    int n, i, found = 0;
    ull pid; 

    Driver* dhead = ReadDriver();
    Passenger* phead = ReadPassenger();
    History* histhead = ReadHistory();
    Status* shead = ReadStatus();
    printf("\n");
    Passenger* pnode = phead;
    Status* nearbyList = null;
    do
    {
        printf("1. Add driver\n2. New booking\n3. Calculate nearest\n");
        printf("4. Most frequently travelled driver-passenger pair\n");
        printf("5. Calculate cost\n6. Show all\nEnter your choice: ");
        scanf("%d", &n);
        switch (n)
        {
        case 1:
            AddDriver(&dhead, &shead);
            UpdateDriver(dhead, shead);
            UpdateStatus(shead);
            break;
        case 2:
            NewBooking(&phead, &histhead, shead, dhead);
            UpdateHistoryPassenger(phead, histhead);
            UpdateStatus(shead);
            break;
        case 3:
            if(phead == null)
            {
                printf("Passenger list empty!");
                break;
            }
            found = 0;
            pnode = phead;
            printf("Enter passenger ID: ");
            scanf("%llu", &pid);
            
            // printf("OUTSIDEEE PNODE ID: %d\n", pnode->p_ID);
            if(pnode->p_ID == pid)
                found = 1;
            else
                pnode = pnode->next;
            while(found==0 && pnode != null)
            {
                // printf("PNODE ID: %d\n", pnode->p_ID);
                if(pnode->p_ID == pid)
                    found = 1;
                else
                    pnode = pnode->next;
            }
            if(found == 0)
                printf("Passenger ID not found!");
            else
            {
                nearbyList = CalcNearby(pnode, shead);
                if(nearbyList == null)
                    printf("No cars nearby!!");
                else
                    printf("List of available drivers:\n");
                while (nearbyList)
                {
                    printf("Driver #%d\n", nearbyList->d_ID);
                    nearbyList = nearbyList->next;
                }
            }
            break;
        case 4:
            MostFrequentPair(histhead, dhead, phead);
            break;
        case 5:
            SuccessfulDrivers(dhead, histhead);
            break;
        case 6:
            showAll();
            break; 
        default:
            break;
        }
        printf("\nDo you want to continue? [y/n]: ");
        scanf("%s", &str);
    }while(str[0]=='y' || str[0]=='Y');
    return 0;
}