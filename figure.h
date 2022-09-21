#pragma once

#include <deque>
#include <mutex>
#include <condition_variable>

namespace antidron_test_task
{
	//вспомогательная структура
	struct pos_t {int x_; int y_;};

	using time_point = std::chrono::system_clock::time_point;
	using milliseconds = std::chrono::milliseconds;
	
	//Класс фигуры
	class figure
	{
	public:
		figure(int x, int y) { x_ = x; y_ = y; };
		
		void set_pos(int x,int y) {
			x_ = x; y_ = y; 
	
			//когда фигура меняет свое положение она выставляет condition_variable
			{
				std::lock_guard lk(cv_m_);
				turn_++;
			}
			cv_.notify_all();
		}

		int get_x() const {return x_;}
		int get_y() const {return y_;}
		int get_turn() const {return turn_;}
		bool wait(int turn, time_point tp){
			//Функция ожидания хода данной фигуры
			std::unique_lock<std::mutex> lk(cv_m_);
			return cv_.wait_until(lk,
						tp,
						[turn,this](){ return turn < this->get_turn(); });
		}

	private:
		int x_;
		int y_;
		int turn_ = 0;

		std::condition_variable cv_;
		std::mutex cv_m_;
	};

	class board
	{
		//Scott Meyers singleton
		board();
		board(const board&) = delete;
		board& operator=(board&) = delete;
	public:
		static board& getInstance() {
			static board  instance;
			return instance;
		}

	public:
		using fig_index = std::deque<figure>::size_type;
		


		//
		void add_figure(const pos_t& pos);
		
		//Generate new random pos, where we want to go
		pos_t new_random_pos(fig_index idx) const;

		//Функция проверяет - свободен ли путь для фигуры с индексом idx, к точке [x:y]
		//Если найдена фигура, которая загораживает путь, то ход не совершается и возвращается индекс
		//блокирующей фигуры (первой найденной)
		//Если путь свободен - фигура idx перемещается в заданную точку, ход считается завершенным
		// и возвращаемое значение - число, превышающее кол-во фигур
		fig_index try_to_move_to(fig_index idx, int x, int y);
		
		//constant reference on figure
		const figure& get_figure(fig_index idx) const;

		void print() const;

		//Функция ожидания, когда фигура с индексом idx совершит ход с номером turn+1
		bool wait_while_figure_make_turn(fig_index idx, int turn, time_point tp);

	private:
		//Используем деку для хранения фигур, так как класс figure содержит condition_variable и mutex,
		//что делает его NotMovable и его объекты не могут быть помещены в std::vector или std::list
		std::deque<figure> figures_;

	};
}

