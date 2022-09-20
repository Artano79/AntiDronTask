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
	std::lock_guard<std::mutex> lock(mut_);

	pos_t result;

	//random value [0-13]
	int rnd14 = std::rand() % 14;
	
	const figure& f = figures_.at(i);

	if(rnd14 < 7){
		//rnd14 - [0-6] - horizontal move
		result.y_ = f.get_pos().y_;
		result.x_ = f.get_pos().x_ < rnd14 ? rnd14 : rnd14 + 1;
	}
	else{
		//rnd14 - [7-13] - vertical move
		rnd14 -= 7;
		result.x_ = f.get_pos().x_;
		result.y_ = f.get_pos().y_ < rnd14 ? rnd14 : rnd14 + 1;
	}

	std::cout << "[" << f.get_pos().x_ << ":" << f.get_pos().y_ <<  "]" << " => ";
	std::cout << "[" << result.x_ << ":" << result.y_ <<  "]" << std::endl;


	return result;
}

bool board::try_to_move_to(int i, const pos_t& dest_pos)
{
	const std::lock_guard<std::mutex> lock(mut_);

	figure& f = figures_.at(i);
	bool is_horizontal = (f.get_pos().y_ == dest_pos.y_);
	
	//runtime check, when figure[i] and dest_pos placed on different lines and columns 
	if(!is_horizontal && f.get_pos().x_ != dest_pos.x_)
		throw std::runtime_error("Bad move");
	
	for(int j = 0; j < figures_.size(); ++j){
		if(j == i)
			continue;
		
		const auto& other_figure = figures_.at(j);
		int left,right,fig_pos;
		
		if(is_horizontal){
			if(f.get_pos().y_ != other_figure.get_pos().y_)
				continue;
			left = f.get_pos().x_;
			right = dest_pos.x_;
			fig_pos = other_figure.get_pos().x_;
		}
		else{
			if(f.get_pos().x_ != other_figure.get_pos().x_)
				continue;
			left = f.get_pos().y_;
			right = dest_pos.y_;
			fig_pos = other_figure.get_pos().y_;
		}

		if(left > right)
			std::swap(left,right);

		//std::cout << is_horizontal << " " << left << " " << right << " " << fig_pos << std::endl;

		if(fig_pos >= left && fig_pos <= right)
		{
			return false;
		}
	}

	
	f.set_pos(dest_pos); 

	return true;
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
	{
		std::cout << "[" << fig.get_pos().x_ << ":" << fig.get_pos().y_ << "]" << std::endl;
		screen[fig.get_pos().x_][fig.get_pos().y_] = 'o';
	}
	
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
