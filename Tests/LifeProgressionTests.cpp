#include "../Source/VincentD/LifeProgression.h"
#include <cassert>
#include <iostream>
using namespace VincentLife;
int main(){
 State s;assert(Valid(s));assert(WorkExtraShift(s)==Result::WrongStage);assert(Deliver(s,true)==Result::WrongStage);assert(Choose(s,Path::City)==Result::WrongStage);
 assert(StartDay(s)==Result::Success);assert(StartDay(s)==Result::AlreadyDone);
 const auto money=s.Money;assert(ServeBreakfast(s,3)==Result::InvalidAnswer&&s.Money==money);
 assert(ServeBreakfast(s,2)==Result::Success);const auto after=s.Money;assert(ServeBreakfast(s,2)==Result::WrongStage&&s.Money==after);
 assert(RepairScooter(s,false)==Result::InvalidAnswer&&!s.BikeReady);State poor=s;poor.Money=40000;assert(RepairScooter(poor,true)==Result::InsufficientFunds);
 assert(RepairScooter(s,true)==Result::Success&&s.HasParcel&&s.BikeReady);assert(Deliver(s,false)==Result::InvalidAnswer&&s.HasParcel);assert(Deliver(s,true)==Result::Success&&!s.HasParcel);
 assert(s.Money==360000);State city=s;assert(Choose(s,Path::Village)==Result::Success&&Valid(s));assert(Choose(city,Path::City)==Result::Success&&Valid(city));assert(Choose(city,Path::Village)==Result::WrongStage&&city.Direction==Path::City);
 assert(BuyPlant(s)==Result::Success&&s.Money==330000);assert(BuyPlant(s)==Result::AlreadyDone&&s.Money==330000);
 s.Energy=0;Sleep(s);assert(s.Energy==100&&Valid(s));assert(Eat(s)==Result::Success&&s.Hunger==100);
 State tired;StartDay(tired);tired.Energy=0;assert(ServeBreakfast(tired,2)==Result::Exhausted);Sleep(tired);assert(ServeBreakfast(tired,2)==Result::Success);
 State copy=s;assert(Valid(copy)&&copy.Current==s.Current&&copy.Direction==s.Direction);copy.Minute=1440;assert(!Valid(copy));copy=s;copy.Money=-1;assert(!Valid(copy));copy=s;copy.HasParcel=true;assert(!Valid(copy));
 State recovery;StartDay(recovery);ServeBreakfast(recovery,2);recovery.Money=0;assert(WorkExtraShift(recovery)==Result::Success&&recovery.Money==40000);assert(WorkExtraShift(recovery)==Result::AlreadyDone&&recovery.Money==40000);Sleep(recovery);Sleep(recovery);Sleep(recovery);assert(WorkExtraShift(recovery)==Result::Success&&recovery.Money==80000);assert(RepairScooter(recovery,true)==Result::Success&&Valid(recovery));
 State time;time.Minute=1400;Advance(time,90);assert(time.Day==2&&time.Minute==50);
 std::cout<<"PASS: task ordering, rewards once, answer validation, affordability, energy recovery, parcel lifecycle, both routes, decoration purchase, time rollover and save-state invariants\n";
}
