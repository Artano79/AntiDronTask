#include <iostream>
#include <list>
#include <algorithm>

#include "figure.h"

using namespace antidron_test_task;


board::board()
{
	//figures_.reserve(figures_count);
}


void board::add_figure(const pos_t& pos)
{
	figures_.emplace_back(pos.x_,pos.y_);
}

pos_t board::new_random_pos(fig_index idx) const
{
	pos_t result;

	//random value [0-13]
	int rnd14 = std::rand() % 14;
	
	const figure& f = figures_.at(idx);

	if(rnd14 < 7){
		//rnd14 - [0-6] - horizontal move
		result.y_ = f.get_y();
		result.x_ = rnd14 < f.get_x() ? rnd14 : rnd14 + 1;
	}
	else{
		//rnd14 - [7-13] - vertical move
		rnd14 -= 7;
		result.x_ = f.get_x();
		result.y_ = rnd14 < f.get_y() ? rnd14 : rnd14 + 1;
	}

	if(f.get_x() == result.x_ && f.get_y() == result.y_)
		throw std::runtime_error("Bad new pos");

	return result;
}

board::fig_index board::try_to_move_to(fig_index idx, int x, int y)
{
	figure& f = figures_.at(idx);
	bool is_horizontal = (f.get_y() == y);
	
	//runtime check, что фигуре задано перемещение по горизонтали или вертикали 
	if(!is_horizontal && f.get_x() != x)
		throw std::runtime_error("Bad move");
	
	std::cout << "Fig " << idx << " ";
	std::cout << "[" << f.get_x() << ":" << f.get_y() <<  "]" << " => ";
	std::cout << "[" << x << ":" << y <<  "] ";


	//Перебираем все фигуры, кроме той что бы двигаем...
	for(fig_index jdx = 0; jdx < figures_.size(); ++jdx){
		if(jdx == idx)
			continue;
		
		//... и проверяем, не стоят ли они на пути
		const auto& other_figure = figures_.at(jdx);
		int left,right,fig_pos;
		
		if(is_horizontal){
			if(f.get_y() != other_figure.get_y())
				continue;
			left = f.get_x();
			right = x;
			fig_pos = other_figure.get_x();
		}
		else{
			if(f.get_x() != other_figure.get_x())
				continue;
			left = f.get_y();
			right = y;
			fig_pos = other_figure.get_y();
		}

		if(left > right)
			std::swap(left,right);

		if(fig_pos >= left && fig_pos <= right)
		{
			std::cout << "FAILED. Blocked by Fig: " << jdx << std::endl;
			//фигура с индексом jdx блокирует путь
			return jdx;
		}
	}

	//Блокирующих фигур не найдено. 
	//внутри будет взведен condition_variable, чтобы оповестить фигуры, которые возможно 
	//ждут что данная фигура разблокирует и путь
	std::cout << "SUCCESS." << std::endl;
	f.set_pos(x,y);

	//Возращаем невалидный индекс
	return figures_.max_size();
}

const figure& board::get_figure(fig_index idx) const
{
	return figures_.at(idx);
}

bool board::wait_while_figure_make_turn(fig_index idx, int turn, time_point tp)
{
	figure& f = figures_.at(idx);
	return f.wait(turn,tp);
}

void board::check_deadblock(fig_index idx,int turns_count)
{
	figure& f = figures_.at(idx);
	int x = f.get_x();
	int y = f.get_y();

	//Лямбда проверяет, что фигура g находится на одной вертикали с фигурой f и слева от нее
	auto lambda_hor_left = [x,y,turns_count](const figure& g){return ((g.get_y() == y) && (g.get_x() == x - 1) && (g.get_turn() >= turns_count));};
	
	//..на одной горизонтали и справа
	auto lambda_hor_right = [x,y,turns_count](const figure& g){return ((g.get_y() == y) && (g.get_x() == x + 1) && (g.get_turn() >= turns_count));};
	
	//на одной вертикали и сверху
	auto lambda_ver_up = [x,y,turns_count](const figure& g){return ((g.get_x() == x) && (g.get_y() == y - 1) && (g.get_turn() >= turns_count));};
	
	//на ожной вертикали и снизу
	auto lambda_ver_down = [x,y,turns_count](const figure& g){return ((g.get_x() == x) && (g.get_y() == y + 1) && (g.get_turn() >= turns_count));};
		
	if(x == 0 || std::find_if(begin(figures_), end(figures_), lambda_hor_left) != figures_.end())
		if(x == 7 || std::find_if(begin(figures_), end(figures_), lambda_hor_right) != figures_.end())
			if(y == 0 || std::find_if(begin(figures_), end(figures_), lambda_ver_up) != figures_.end())
				if(y == 7 || std::find_if(begin(figures_), end(figures_), lambda_ver_down) != figures_.end())
				{
					f.set_turn(turns_count);
					throw std::runtime_error("Figure in dead-block");
				}
}

void board::print() const
{
	//простейший способ вывода на экран состояния фигур
	static std::array<std::array<char,8>,8> screen;

	for(auto& line : screen)
		for(auto& c : line)
			c = ' ';

	for(auto& fig : figures_)
	{
		std::cout << "[" << fig.get_x() << ":" << fig.get_y() << "]" << std::endl;
		screen[fig.get_x()][fig.get_y()] = 'o';
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
