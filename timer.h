#pragma once
// 对于通用类,只需扩展方法逻辑而无需扩展数据成员的情况下,我们使用回调函数的方法会更加简洁和方便, 此时无需继承父类
#include <functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void restart() {
		pass_time = 0;
		is_triggered = false;
	}

	void set_wait_time(int val) {
		wait_time = val;
	}

	void set_one_shot(bool flag) {
		is_one_shot = flag;
	}

	void pause() {
		is_paused = true;
	}

	void resume() {
		is_paused = false;
	}

	void on_update(int delta){
		// some logic to trigger callback
		if (is_paused) {
			return;
		}

		pass_time += delta;

		if (pass_time >= wait_time) {
			if ((!is_one_shot || (is_one_shot && !is_triggered)) && callback) {
				callback();
				is_triggered = true;
				pass_time = 0;
			}
		}
	}

	void set_callback(std::function<void()> callback) {
		this->callback = callback;
	}

private:
	int pass_time = 0; // 已过时间
	int wait_time = 0; // 等待时间
	bool is_paused = false; // 是否暂停
	bool is_triggered = false; // 是否触发
	bool is_one_shot = false; // 是否单次触发
	std::function<void()> callback;
};

