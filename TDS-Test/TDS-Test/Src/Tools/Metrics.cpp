#include "Metrics.h"
#include <iostream>

FuncTest::FuncTest() {};
FuncTest::FuncTest(std::string name) : name(name) {}

void FuncTest::Reset() {
	funcCalls = 0;
	funcTime = std::chrono::high_resolution_clock::duration();
}

void FuncTest::Start()
{
	funcCalls++;
	funcStartTime = std::chrono::high_resolution_clock::now();
}

void FuncTest::End()
{
	funcTime += std::chrono::high_resolution_clock::now() - funcStartTime;
}

void FuncTest::Log(double totalTime) const {
	double tTime = std::chrono::duration_cast<std::chrono::nanoseconds>(funcTime).count();
	std::cout << "	" << name << " || total Time = " << tTime << " <--> " << tTime / totalTime * 100 << " | function Calls = " << funcCalls << " | avr. Time = " << tTime / funcCalls << std::endl;
}

Metric::Metric(std::string name, std::map<std::string, FuncTest> feelers) : name(name), firstRun(true), feelers(feelers) {}

Metric::~Metric()
{
}

void Metric::Loop() {
	if (!firstRun) {
		loopTime = std::chrono::high_resolution_clock::now() - loopStartTime;
		Log();
	}
	else {
		firstRun = false;
	}
	for (auto &fT : feelers) { fT.second.Reset(); }
	loopStartTime = std::chrono::high_resolution_clock::now();
}

void Metric::LoopBegin()
{
	for (auto &fT : feelers) { fT.second.Reset(); }
	loopStartTime = std::chrono::high_resolution_clock::now();
}

void Metric::LoopEnd()
{
	loopTime = std::chrono::high_resolution_clock::now() - loopStartTime;
	Log();
}

void Metric::Log() const
{
	double tLoopTime = std::chrono::duration_cast<std::chrono::nanoseconds>(loopTime).count();
	
	std::cout << "==================================================================" << std::endl;
	std::cout << name << " | loopTime = " << tLoopTime << std::endl;
	for (const auto& fT : feelers) {
		fT.second.Log(tLoopTime);
	}
	std::cout << "==================================================================" << std::endl;
}

std::string Metric::GetName() const { return this->name; }

// static funcitons
Metric Metrics::sMetrics[] {
	Metric("default", std::map<std::string, FuncTest> {}),
	Metric("GameMetric", std::map<std::string, FuncTest> { {"SightCalc", FuncTest("SightCalc")} }),
	Metric("SightMetric", std::map<std::string, FuncTest> { {"Hitscan", FuncTest("Hitscan")},{ "Sorting", FuncTest("Sorting") }, { "Adding", FuncTest("Adding") },{ "CreatingTriangles", FuncTest("CreatingTriangles") }})
};

Metric& Metrics::GetMetric(std::string name)
{
	for (Metric &m : sMetrics) {
		if (m.GetName() == name) {
			return m;
		}
	}
	std::cout << "METRICS::METRIC NOT FOUND" << std::endl;
	return sMetrics[0];
}