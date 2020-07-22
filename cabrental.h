#include <stdio.h>
#include <string.h>
#define ull unsigned long long
#define SIZE 100

typedef struct Location{
    float x, y;
} Location;

typedef struct DriverNode{
    ull d_ID;
    char name[SIZE];
    char vehicletype[SIZE];   
    // a = auto, b = bike, c = car

    struct DriverNode* next;
    struct DriverNode* prev;
}Driver;    // driver details node

typedef struct PassengerNode{
    ull p_ID;
    ull p_mobile_no;
    struct Location p_location, drop_location;
    char name[SIZE];

    struct PassengerNode* next;
    struct PassengerNode* prev;
}Passenger; // passenger details node (sorted linked list)

typedef struct Booking_History{
    ull d_ID, p_ID;
    ull p_mobile_no;
    ull frequency;
    float distance_travelled;

    struct Booking_History* next;
    struct Booking_History* prev;
}History;   // node containing history of all trips per driver-passenger pair

typedef struct Current_Status{
    ull d_ID;
    Location location;  
    ull status;     // booked = 1, free = 0

    struct Current_Status* next;
    struct Current_Status* prev;
}Status;    // node tracking current status of each driver

float CalcDistance(Location,Location);
int _leastIndex(float,float,float);
float CalculateCost (Driver* dptr, History* histhead);
void GetPassengerDetails(Passenger* p, ull pid);
Passenger* AddPassenger (Passenger** head, ull pid);
void AddHistory(History** histhead, ull driverID, Passenger* pnode);
void GetDriverDetails(Driver* d, Status* s);
void FinishTrip (ull driverID, Status* shead);