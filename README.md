# Cab Rental System in C
This project is an implementation of a Cab Rental System in C using linked lists, with the following features:

1. **Adding a new driver** to the database (and writing this data to a text file)  

2. **Making a new booking**:  
    1. User **inputs passenger ID**.  
        * If that passenger is found in passengers.txt, then **asks the user to enter a pickup location and a drop location** (as coordinates).  
        * If passenger ID isn’t found, then makes a new account with entered ID as the new passenger’s ID. **Asks for passenger name, mobile number, and pick up and drop location**.  
    2. The program then **assigns the nearest driver** that is free to the passenger, informing the user of the name of the driver and the vehicle that the driver is arriving in.  
    3. The user is asked if they want to end the trip and free the driver.  
        * If the answer is yes, then the status of the driver is made free again and the history.txt **file is updated**, with the information of the trip just completed.  
        * If the answer is no, the driver status remains occupied.  
        
3. **Calculating nearest drivers** - this prints the driver IDs of all free drivers within a 5-kilometre radius of the location of the passenger.  

4. **Displaying the most frequent driver-passenger pair** - every time a trip is booked, the frequency of the driver-passenger pair travelling together is incremented. This option prints the name and IDs of the most frequent pair, and also displays the frequency.  

5. **Calculating total earnings of drivers** - computes the total earnings of all the drivers, and displays the three most successful drivers with their name and IDs. (earning is calculated by multiplying the total distance travelled by a driver with a constant price)  

6. **Show all** - this displays in a pretty format the contents of the driver.txt, passenger.txt, status.txt and history.txt files. (basically all info currently in the “database”)  

This program is purely console-based and has a user-friendly interactive menu that lets the user select a choice, displays useful error information if invalid inputs are entered, and prints some verbose output in certain situations like when the history file is updated.
  
