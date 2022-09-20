#pragma once

#include <vector>
#include <mutex>

namespace antidron_test_task
{
	struct pos_t{ int x_; int y_; };

	class figure
	{
	public:
		figure(const pos_t& pos) { pos_ = pos; };
		const pos_t get_pos() const { return pos_; }
		void set_pos(const pos_t& pos) {pos_ = pos;};
	private:
		pos_t pos_;
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
		//
		void add_figure(const pos_t& pos);
		
		//Generate new random pos, where we want to go
		pos_t new_random_pos(int i) const;

		//Check - is way blocked by other figures
		bool try_to_move_to(int i, const pos_t& dest_pos);
		
		//constant reference on figure
		const figure& get_figure(int i) const;

		void print() const;

	private:
		std::vector<figure> figures_;
		mutable std::mutex mut_;

	};
}

