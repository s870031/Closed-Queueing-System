// Author: Hsin Hua Chen

// Simulates an closed queueing system.  The simulation terminates
//  once 100000 customers depart from the system.

#include <iostream>
#include <iomanip>
#include "rv.h"
#include "event.h"
using namespace std;

enum {ARR,DEP};                 // Define the event types
enum {H,L};                     // Define High, Low priorities
enum {Q1,Q2};

//2015/12/6 add for proj2
  double PH;                // Probability that arrival is high priority
  double PL;                // Probability that arrival is low priority
  double r2d;
  double r21;
  double r22;
  double mu1;
  double mu2H;
  double mu2L;

  EventList Elist;                // Create event list

struct Data
{
   double theta1H_theo[11];
   double theta1L_theo[11];
   double theta2H_theo[11];
   double theta2L_theo[11];
   double EN1H_theo[11];
   double EN1L_theo[11];
   double EN2H_theo[11];
   double EN2L_theo[11];
   double ES2H_theo[11];
   double ES2L_theo[11];

   double theta1H_sim[11];
   double theta1L_sim[11];
   double theta2H_sim[11];
   double theta2L_sim[11];
   double EN1H_sim[11];
   double EN1L_sim[11];
   double EN2H_sim[11];
   double EN2L_sim[11];
   double ES2H_sim[11];
   double ES2L_sim[11];
};

void genARR(double time){    // generate next Arrival

  if(uni_rv() <= PH)
    Elist.insert(time, ARR, H, Q1, 0);    
  else
    Elist.insert(time, ARR, L, Q1, 0);
}
void genDEP(double time, int priority, int queue){
  Elist.insert(time, DEP, priority, queue, 0);         // generate next departure 
}

int main()
{
  Data data;   

  //------


  // // ****** Input Setting ******
  
  //2015/12/6 add for proj2
   cout << "PH  : "; cin >> PH;
   cout << "PL  : "; cin >> PL;
   cout << "r2d : "; cin >> r2d;
   cout << "r21 : "; cin >> r21;
   cout << "r22 : "; cin >> r22;
   cout << "mu1 : "; cin >> mu1;
   cout << "mu2H: "; cin >> mu2H;
   cout << "mu2L: "; cin >> mu2L;
  //PH=0.4; PL=0.6; r2d=0.25; r21=0.25; r22=0.5; mu1=20; mu2H=10; mu2L=50;

  // // ******* Simulation ******* 
  for(int lambda=1; lambda<=10; lambda++)
  {

  double clock = 0.0;             // System clock
  int N = 0;                      // Number of customers in system
  int Ndep = 0;                   // Number of departures from system

  int NH1 = 0;
  int NL1 = 0;
  int NH2 = 0;
  int NL2 = 0;
  int N1 = 0;
  int N2 = 0;

  int done = 0;                   // End condition satisfied?
  int priority = 0;
  int queue = 0;
  int goback = 0;

  int ARR1H = 0;                 // number of High priority Arrival in queue 1
  int ARR1L = 0;                 // number of Low priority Arrival in queue 1
  int ARR2H = 0;                 // number of High priority Arrival in queue 2
  int ARR2L = 0;                 // number of Low priority Arrival in queue 2
  int EN1H_sim = 0;
  int EN1L_sim = 0;
  int EN2H_sim = 0;
  int EN2L_sim = 0;

  Event* CurrentEvent;
  
  
  for(int i=0; i<10; i++)genARR(clock+exp_rv(lambda));
  
  while (!done)
  {
    CurrentEvent = Elist.get();               // Get next Event from list
    double prev = clock;                      // Store old clock value
    clock=CurrentEvent->time;                 // Update system clock

    priority = CurrentEvent->priority;           // Job priority
    queue = CurrentEvent->queue;                 // Job in which queue
    goback = CurrentEvent->goback;               // if Job is recycle?

    switch (CurrentEvent->type) {
    // Arrival
    case ARR:                                    // If arrival

      N1 = NH1 + NL1;
      N2 = NH2 + NL2;
      EN1H_sim += NH1*(clock-prev);
      EN1L_sim += NL1*(clock-prev);
      EN2H_sim += NH2*(clock-prev);
      EN2L_sim += NL2*(clock-prev);

      if(queue == Q1)
      {
          //cout << "ARR in Q1;       " << "NH1:" << NH1 << ";  NL1:" << NL1 << ";  NH2:" << NH2 << ";  NL2:" << NL2 << endl;


          if(priority == H)                             // update size
          {
             NH1++;
             ARR1H++;                                    
          }
          else if(priority == L)
          {
             NL1++;
             ARR1L++;
          }
          if(N1 == 0)                                 // if server is idle
          {
             genDEP(clock+exp_rv(mu1),priority,Q1);   //    generate Departure
          }

      }
      else if(queue == Q2)
      {
          //cout << "ARR in Q2;       " << "NH1:" << NH1 << ";  NL1:" << NL1 << ";  NH2:" << NH2 << ";  NL2:" << NL2 << endl;


          if(priority == H)                             // update size
          {
             NH2++;
             ARR2H++;                                    
          }
          else if(priority == L)
          {
             NL2++;
             ARR2L++;
          }
          if(N2 == 0)                                 // if server is idle
          {
            if(priority == H)
             genDEP(clock+exp_rv(mu2H),priority,Q2);   //    generate Departure
           else if(priority == L)
             genDEP(clock+exp_rv(mu2L),priority,Q2);   //    generate Departure
          }

      }

      if(goback == 0)
        genARR(clock+exp_rv(lambda));                       // generate Arrival if applicable

      break;
    
    // Depature
    case DEP:                                 // If departure

      N1 = NH1 + NL1;
      N2 = NH2 + NL2;

      EN1H_sim += NH1*(clock-prev);
      EN1L_sim += NL1*(clock-prev);
      EN2H_sim += NH2*(clock-prev);
      EN2L_sim += NL2*(clock-prev);

      if(queue == Q1)
      {
          //cout << "DEP in Q1;       " << "NH1:" << NH1 << ";  NL1:" << NL1 << ";  NH2:" << NH2 << ";  NL2:" << NL2 << endl;

          if(priority == H)                                  // update size
            NH1--;
          else if(priority == L)
            NL1--;

          if(NH1 > 0)                                        // if High priority job in queue1
            genDEP(clock+exp_rv(mu1),H,Q1);           //     generate departure
          else if(NL1 > 0)                                   // if Low  priority job in queue1
            genDEP(clock+exp_rv(mu1),L,Q1);           //     generate departure

          if(N2 == 0)                                        // if queue2 is empty
          {
            if(priority == H)
              genDEP(clock+exp_rv(mu2H),H,Q2);        //     generate departure
            else if(priority == L)
              genDEP(clock+exp_rv(mu2L),L,Q2);        //     generate departure
          }

          (priority == H) ? NH2++ : NL2++ ;
          (priority == H) ? ARR2H++ : ARR2L++;
      }
      else if(queue == Q2)
      {
          //cout << "DEP in Q2;      " << "NH1:" << NH1 << ";  NL1:" << NL1 << ";  NH2:" << NH2 << ";  NL2:" << NL2 << endl;

          if(priority == H)                                  // update size
          {
            NH2--;
            Ndep++;                                          // depart from system
          }
          else if(priority == L)
          {
            NL2--;
          }

          if(NH2 > 0)                                        // if High priority job in queue2
          {
            genDEP(clock+exp_rv(mu2H),H,Q2);          //     generate departure 
          }
          else if(NL2 > 0)                                   // if Low priority job in queue2
          {
            genDEP(clock+exp_rv(mu2L),L,Q2);          //      generate departure 
          }

          if(priority == L)
          {
            int p = uni_rv();
            if(p <= r2d)                                     //    P = r2d:
              Ndep++;                                        //      depart from system
            else if(r2d <= p && p <= (r21+r2d))              //    P = r21:
              Elist.insert(clock, ARR, priority, Q1, 1);     //      depart to queue1
            else if(p >= (r21+r2d))                          //    P = r22:
              Elist.insert(clock, ARR, priority, Q2, 1);     //      depart to queue2
          }
      }
      
      break;
    }

    delete CurrentEvent;
    if (Ndep > 1000) done=1;        // End condition

    //cout << "Ndep: "<< Ndep << endl;
  }





  // ****** Theoretical value ******
  double theta1H, theta2H, theta1L, theta2L;      // Throughput
  double theta1, theta2;
  double rho1H,rho1L, rho2H, rho2L;
  double EN1H, EN1L, EN2H, EN2L;                  // Expected customer in queue
  double ES2H, ES2L;                              // Average service time
  
     
    // Throughput
    theta1H = lambda*PH;                           // Traffic equations
    theta2H = theta1H;                             // Traffic equations
    theta2L = lambda*PL / (1-r21-r22);             // Traffic equations
    theta1L = lambda*PL + theta2L*r21;             // Traffic equations

    // Expected number
    rho1H = theta1H / mu1;
    rho1L = theta1L / mu1;
    rho2H = theta2H / mu2H;
    rho2L = theta2L / mu2L;

    EN1H = rho1H / (1-rho1H);                      // View as M/M/1 system
    EN1L = rho1L / (1-rho1L);
    EN2H = rho2H / (1-rho2H);
    EN2L = rho2L / (1-rho2L);

    // Average time
    ES2H = EN2H / theta2H;                        // Little's law
    ES2L = EN2L / theta2L;                        // Little's law

    //******* Save Theoretical value **********
    data.theta1H_theo[(int)lambda] =  theta1H;
    data.theta1L_theo[(int)lambda] = theta1L;
    data.theta2H_theo[(int)lambda] = theta2H;
    data.theta2L_theo[(int)lambda] = theta2L;
    
    data.EN1H_theo[(int)lambda] = EN1H;
    data.EN1L_theo[(int)lambda] = EN1L;
    data.EN2H_theo[(int)lambda] = EN2H;
    data.EN2L_theo[(int)lambda] = EN2L;

    data.ES2H_theo[(int)lambda] = ES2H;
    data.ES2L_theo[(int)lambda] = ES2L;

    data.theta1H_sim[(int)lambda] = ARR1H/clock;
    data.theta1L_sim[(int)lambda] = ARR1L/clock;
    data.theta2H_sim[(int)lambda] = ARR2H/clock;
    data.theta2L_sim[(int)lambda] = ARR2L/clock;

    data.EN1H_sim[(int)lambda] = EN1H_sim/clock;
    data.EN1L_sim[(int)lambda] = EN1L_sim/clock;
    data.EN2H_sim[(int)lambda] = EN2H_sim/clock;
    data.EN2L_sim[(int)lambda] = EN2L_sim/clock;

    data.ES2H_sim[(int)lambda] = EN2H_sim/(double)ARR2H;
    data.ES2L_sim[(int)lambda] = EN2L_sim/(double)ARR2L;




  }

  // *********** OUTPUT ************
  //Throughput
  int w=15;
  cout << endl <<"============= OUTPUT =================" << endl << endl;
  cout << "----- Throughput ----- " << endl;
  cout << setw(5) << "Lamda" 
       << setw(w) << "Theta1H(S)"
       << setw(w) << "Theta1H(T)"
       << setw(w) << "Theta1L(S)" 
       << setw(w) << "Theta1L(T)" 
       << setw(w) << "Theta2H(S)"
       << setw(w) << "Theta2H(T)" 
       << setw(w) << "Theta2L(S)"
       << setw(w) << "Theta2L(T)" << endl;
  
  for(int i=1; i<=10; i++){
    cout << setw(5) << i 
         << setw(w) << data.theta1H_sim[i]
         << setw(w) << data.theta1H_theo[i] 
         << setw(w) << data.theta1L_sim[i]
         << setw(w) << data.theta1L_theo[i] 
         << setw(w) << data.theta2H_sim[i]
         << setw(w) << data.theta2H_theo[i] 
         << setw(w) << data.theta2L_sim[i]
         << setw(w) << data.theta2L_theo[i] << "\n"; 
  }

  //Excepted number
  cout << endl << endl;
  cout << "----- Expected number ----- " << endl;
  cout << setw(5) << "Lamda" 
       << setw(w) << "EN1H(S)"
       << setw(w) << "EN1H(T)" 
       << setw(w) << "EN1L(S)"
       << setw(w) << "EN1L(T)" 
       << setw(w) << "EN2H(S)"
       << setw(w) << "EN2H(T)" 
       << setw(w) << "EN2L(S)"
       << setw(w) << "EN2L(T)" << endl;
  
  for (int i=1; i<=10; i++){
    cout << setw(5) << i 
         << setw(w) << data.EN1H_sim[i]
         << setw(w) << data.EN1H_theo[i] 
         << setw(w) << data.EN1L_sim[i]
         << setw(w) << data.EN1L_theo[i] 
         << setw(w) << data.EN2H_sim[i]
         << setw(w) << data.EN2H_theo[i] 
         << setw(w) << data.EN2L_sim[i] 
         << setw(w) << data.EN2L_theo[i]<< "\n"; 
  }

  // Average time
  cout << endl;
  cout << "----- Average time in Queue 2 ----- " << endl;
  cout << setw(5) << "Lamda" 
       << setw(w) << "ES2H(S)"
       << setw(w) << "ES2H(T)" 
       << setw(w)<< "ES2L(S)"
       << setw(w)<< "ES2L(T)" << endl;

  for(int i=1; i<=10; i++){
    cout << setw(5) << i 
         << setw(w) << data.ES2H_sim[i]
         << setw(w) << data.ES2H_theo[i] 
         << setw(w) << data.ES2L_sim[i]
         << setw(w) << data.ES2L_theo[i] << endl;
  }

  cout << endl << endl;
}

