#include <iostream>

#include "figure.h"

using namespace antidron_test_task;


board::board()
{
	//figures_.reserve(figures_count);
}


void board::add_figure(const pos_t& pos)
{
	figures_.emplace_back(pos);
}

pos_t board::new_random_pos(int i) const
{
	//std::rand
	return pos_t();
}

bool board::try_to_move_to(int figure, const pos_t& dest_pos)
{
	return false;
}

const figure& board::get_figure(int i) const
{
	return figures_.at(i);
}

void board::print() const
{
	std::array<std::array<char,8>,8> screen;

	for(auto& line : screen)
		for(auto& c : line)
			c = ' ';

	for(auto& fig : figures_)
		screen[fig.get_pos().x_][fig.get_pos().y_] = 'o';

	std::cout << "-------------------" << std::endl;
	for(int y = 0 ; y < 8; ++y)
	{
		std::cout << "| ";
		for(int x = 0 ; x < 8; ++x)
			std::cout << screen[x][y] << " ";		

		std::cout << "|" <<std::endl;
	}

	std::cout << "-------------------" << std::endl;
}
