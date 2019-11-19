// MainProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <memory>
#include "PrimitiveTypes2.h"
#include "Settings.h"
#include <stdexcept>
#include "Parser.h"
#include <iterator>
//#include "TemplatesH.h"

using namespace std;


//template<typename T>
//T functestt(T var);
//extern template struct Identity<double>;
//template <typename T>
//T functest02(typename Identity<T>::type arg);

int main(int argc, char**argv)
{
    //std::cout << "Hello World!\n";

	//wstring wstr(L"b");
	//Symbol s(wstr);

	//double k = 5;
	//double in = functest02<double>(k);
	//double in = functestt(k);
	
	//Scalar s1(L"w");
	//Scalar s2(L"W");
	//bool b=s1 == s2;

	/*Vecdotted v(L"a", L"b");*/

	//Unit u;
	//u.multiplyByVec(L"a");
	//Vec v(L"s");
	//u.multiplyByVec(v);
	//u.multiplyByVec(Vec{L"f"});

	//std::queue<int> q;
	//q.emplace(1);
	//q.emplace(2);
	//int & in = q.front();
	//q.pop();
	//std::cout << in << std::endl;

	//Unit u;



	//auto tmp = Settings::tolerance;
	//int64_t in = 5;
	//int64_t in2 = std::move(in);

	//std::list<int> lint;
	//lint.push_back(1);
	//lint.push_back(2);
	//lint.push_back(3);
	//lint.push_back(4);
	//auto e = lint.end();
	//auto bi = lint.begin();
	//++bi;
	//auto itnext = bi;
	//++itnext;
	//auto newe = e;
	//--newe;
	//lint.erase(bi);
	//cout << *itnext << endl;
	//cout << *newe << endl;

	//std::list<int> l={1,2};
	//std::list<int> l2 = {0};
	//l.splice(l.begin(), l2);
	//std::copy(l.begin(), l.end(), std::ostream_iterator<int>(cout," "));

	{//1
		//std::unique_ptr<Unit> umain(new Unit);
		//{
		//	std::unique_ptr<Unit> u1(new Unit);
		//	u1->m_m.m_sg->addScalar(L"a");
		//	u1->m_m.m_sg->addMultipe(5);
		//	std::unique_ptr<Unit> u11(new Unit);
		//	u11->multiplyByVec(L"v");
		//	std::unique_ptr<Unit> u12(new Unit);
		//	u12->multiplyByVec(L"v");
		//	u12->m_m.m_sg->addMultipe(2);
		//	std::list<unique_ptr<Unit>> ltmp;
		//	ltmp.emplace_back(std::move(u11));
		//	ltmp.emplace_back(std::move(u12));
		//	u1->multiplyBySum(ltmp);

		//	umain->m_s.emplace_back(std::move(u1));
		//}
		//{

		//	std::unique_ptr<Unit> u1(new Unit);
		//	u1->m_m.m_sg->addScalar(L"a");
		//	u1->m_m.m_sg->addMultipe(3);
		//	{
		//		std::unique_ptr<Unit> u11(new Unit);
		//		u11->multiplyByVec(L"v");
		//		std::unique_ptr<Unit> u12(new Unit);
		//		std::list<unique_ptr<Unit>> ltmp;
		//		ltmp.emplace_back(std::move(u11));
		//		ltmp.emplace_back(std::move(u12));
		//		u1->multiplyBySum(ltmp);
		//	}
		//	{
		//		std::unique_ptr<Unit> u21(new Unit(5));
		//		u21->multiplyByVec(L"v");
		//		std::unique_ptr<Unit> u22(new Unit);
		//		u22->m_m.m_sg->addScalar(L"a");
		//		std::list<unique_ptr<Unit>> ltmp;
		//		ltmp.emplace_back(std::move(u21));
		//		ltmp.emplace_back(std::move(u22));
		//		u1->multiplyBySum(ltmp);
		//	}


		//	umain->m_s.emplace_back(std::move(u1));
		//}
		//umain->expand();
	}
	try
	{
		if (argc > 1)
			handle_input(argv[1]);
		else
			cout << "Must specify input file name." << endl;
	}
	catch (string const& str)
	{
		cout << str << endl;
	}
	catch (std::exception const & e)
	{
		cout << e.what() << endl;
	}
	catch (...)
	{
		cout << "unknown exception" << endl;
	}
	//for (auto& pair : lv.m_lv)
	//{
	//	if (pair.second)
	//	{
	//		pair.second->expand();
	//		printu(pair.first, *pair.second);
	//	}

	//}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
