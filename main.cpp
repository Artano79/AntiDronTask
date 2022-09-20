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
	
	int turn = 0;
	while (turn < turns_count)
	{
		//Every even turn we make sleep for a random time [200-300ms]
		if (turn % 2 > 0)
			std::this_thread::sleep_for(milliseconds(200 + std::rand() % 300));

		//make destination pos
		const pos_t destination_pos = b.new_random_pos(index);
		
		bool way_is_blocked = true;
		if(!(way_is_blocked = b.try_to_move_to(index, destination_pos)))
		{
			std::cout << "Fig " << index << " stuck on turn " << turn << std::endl;
		
			const auto start_time = std::chrono::high_resolution_clock::now();
			while (way_is_blocked)
			{
				const auto now_time = std::chrono::high_resolution_clock::now();
				const auto us = std::chrono::duration_cast<milliseconds>(now_time - start_time).count();

				if (us > 500)
					break;

				std::this_thread::sleep_for(milliseconds(1));

				way_is_blocked = !(b.try_to_move_to(index, destination_pos));
			}
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
		std::array<pos_t, figures_count> initial_pos{ { {1,1}, {2,1}, {3,7}, {6,2}, {1,0} } };
	
		//Creating figures on board
		for (pos_t& pos : initial_pos)
			b.add_figure(pos);

		b.print();
		std::cout << std::endl;


		/*{
			bool test_result = b.try_to_move_to(0, pos_t{2,1});
			std::cout << "Test Result = " << test_result << std::endl;
			return 0;

		}*/




		//Start threads
		std::list<std::thread> threads;
		for (int i = 0; i < figures_count; i++)
			threads.emplace_back(figure_thread_func, i);

		std::cout << "waiting for helpers to finish..." << std::endl;
	
		//Waiting for all thread done
		for (auto& t : threads)
			t.join();
	
		std::cout << "t1 finished..." << std::endl;

		b.print();
	}
	catch (const std::exception& e)
	{
		std::cout << "unexpected exception: " << e.what() << std::endl;
	}
}

