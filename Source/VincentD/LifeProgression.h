#pragma once
#include <cstdint>

// Engine-independent rules are exercised by Tests/LifeProgressionTests.cpp.
namespace VincentLife {
enum class Quest : int { Home, Breakfast, Scooter, Delivery, Decision, FreePlay };
enum class Path : int { Undecided, Village, City };
enum class Result { Success, WrongStage, InvalidAnswer, InsufficientFunds, Exhausted, AlreadyDone };
struct State {
 std::int64_t Money=250000;
 int Day=1,Minute=420,Energy=80,Hunger=75;
 int MotherTrust=0,MechanicTrust=0,FriendTrust=0,LastExtraShiftDay=0;
 Quest Current=Quest::Home;
 Path Direction=Path::Undecided;
 bool BikeReady=false,HasParcel=false,FoundNotebook=false,OwnsPlant=false;
};
inline void Advance(State& S,int Minutes,int Effort=0){
 const int Before=S.Minute;
 S.Minute+=Minutes;while(S.Minute>=1440){S.Minute-=1440;++S.Day;}
 S.Energy=S.Energy>Effort?S.Energy-Effort:0;
 const int Food=Minutes/30+((Before%30+Minutes%30)>=30?1:0);S.Hunger=S.Hunger>Food?S.Hunger-Food:0;
}
inline Result StartDay(State& S){
 if(S.Current!=Quest::Home)return Result::AlreadyDone;
 S.Current=Quest::Breakfast;Advance(S,10);return Result::Success;
}
inline Result ServeBreakfast(State& S,int Bowls){
 if(S.Current!=Quest::Breakfast)return Result::WrongStage;
 if(Bowls!=2)return Result::InvalidAnswer;
 if(S.Energy<15)return Result::Exhausted;
 S.Money+=70000;++S.MotherTrust;Advance(S,90,15);S.Current=Quest::Scooter;return Result::Success;
}
inline Result WorkExtraShift(State& S){
 if(int(S.Current)<int(Quest::Scooter))return Result::WrongStage;
 if(S.LastExtraShiftDay==S.Day)return Result::AlreadyDone;
 if(S.Energy<20)return Result::Exhausted;
 S.LastExtraShiftDay=S.Day;S.Money+=40000;Advance(S,120,20);return Result::Success;
}
inline Result RepairScooter(State& S,bool CorrectDiagnosis){
 if(S.Current!=Quest::Scooter)return Result::WrongStage;
 if(!CorrectDiagnosis)return Result::InvalidAnswer;
 if(S.Money<50000)return Result::InsufficientFunds;
 if(S.Energy<10)return Result::Exhausted;
 S.Money-=50000;S.BikeReady=true;++S.MechanicTrust;S.HasParcel=true;Advance(S,45,10);S.Current=Quest::Delivery;return Result::Success;
}
inline Result Deliver(State& S,bool CorrectAddress){
 if(S.Current!=Quest::Delivery||!S.BikeReady||!S.HasParcel)return Result::WrongStage;
 if(!CorrectAddress)return Result::InvalidAnswer;
 if(S.Energy<10)return Result::Exhausted;
 S.HasParcel=false;S.Money+=90000;++S.FriendTrust;Advance(S,40,10);S.Current=Quest::Decision;return Result::Success;
}
inline Result Choose(State& S,Path Choice){
 if(S.Current!=Quest::Decision)return Result::WrongStage;
 if(Choice!=Path::Village&&Choice!=Path::City)return Result::InvalidAnswer;
 S.Direction=Choice;S.Current=Quest::FreePlay;Advance(S,15);return Result::Success;
}
inline Result BuyPlant(State& S){
 if(S.OwnsPlant)return Result::AlreadyDone;
 if(S.Money<30000)return Result::InsufficientFunds;
 S.Money-=30000;S.OwnsPlant=true;return Result::Success;
}
inline Result Eat(State& S){
 if(S.Money<20000)return Result::InsufficientFunds;
 S.Money-=20000;Advance(S,15);S.Hunger=100;return Result::Success;
}
inline void Sleep(State& S){Advance(S,480);S.Energy=100;}
inline bool Valid(const State& S){
 return S.Money>=0&&S.Money<=1000000000000LL&&S.Day>=1&&S.Day<=100000&&S.Minute>=0&&S.Minute<1440&&S.Energy>=0&&S.Energy<=100&&S.Hunger>=0&&S.Hunger<=100&&int(S.Current)>=0&&int(S.Current)<=5&&int(S.Direction)>=0&&int(S.Direction)<=2&&S.MotherTrust>=0&&S.MechanicTrust>=0&&S.FriendTrust>=0&&S.LastExtraShiftDay>=0&&S.LastExtraShiftDay<=S.Day&&
 (int(S.Current)<int(Quest::Delivery)||S.BikeReady)&&
 (S.HasParcel==(S.Current==Quest::Delivery))&&
 ((S.Current==Quest::FreePlay)==(S.Direction!=Path::Undecided));
}
}
