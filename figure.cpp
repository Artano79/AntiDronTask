#include "figure.h"

using namespace antidron_test_task;


board::board()
{
	//figures_.reserve(figures_count);
}


void antidron_test_task::board::add_figure(const pos_t& pos)
{
	figures_.emplace_back(pos);
}

pos_t board::new_random_pos(int i) const
{
	std::rand
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


