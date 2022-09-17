// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <array>
#include <thread>
#include <mutex>
#include <list>

#include "figure.h"


using namespace antidron_test_task;

//consts
static const int turns_count = 50;
static const int figures_count = 5;

//pseudonames
//using clock = std::chrono::high_resolution_clock;
using milliseconds = std::chrono::milliseconds;

void figure_thread_func(int index)
{
	//std::unique_lock<std::mutex> lock{ mut, std::defer_lock };

	board &b = board::getInstance();
	const figure &f = b.get_figure(index);

	int turn = 0;
	while (turn < turns_count)
	{
		//Every even turn we make sleep for a random time [200-300ms]
		if (turn % 2 > 0)
			std::this_thread::sleep_for(milliseconds(200 + std::rand() % 300));

		//make destination pos
		const pos_t destination_pos = b.new_random_pos(index);
		
		const auto start_time = std::chrono::high_resolution_clock::now();
		bool way_is_blocked;
		while (!(way_is_blocked = b.try_to_move_to(index, destination_pos)))
		{
			const auto now_time = std::chrono::high_resolution_clock::now();
			const auto us = std::chrono::duration_cast<milliseconds>(start_time - now_time).count();

			if (us > 5000)
				break;

			std::this_thread::sleep_for(milliseconds(1));
		}

		if (!way_is_blocked)
		{
			turn++;
		}
	}
	
	//lock.lock();
	std::cout << "thread finished..." << std::endl;
}



int main()
{
	try
	{
		//Construct board singleton-object
		board &b = board::getInstance();
	
		//Initial positions of figures
		std::array<pos_t, figures_count> initial_pos{ { {0,0}, {1,1}, {0,0}, {0,0}, {0,0} } };
	
		//Creating figures on board
		for (pos_t& pos : initial_pos)
			b.add_figure(pos);

		//Start threads
		std::list<std::thread> threads;
		for (int i = 0; i < figures_count; i++)
			threads.emplace_back(figure_thread_func, i);

		std::cout << "waiting for helpers to finish..." << std::endl;
	
		//Waiting for all thread done
		for (auto& t : threads)
			t.join();
	
		std::cout << "t1 finished..." << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "unexpected exception: " << e.what() <<std::endl;
	}
}

