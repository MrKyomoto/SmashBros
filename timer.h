#pragma once
// ����ͨ����,ֻ����չ�����߼���������չ���ݳ�Ա�������,����ʹ�ûص������ķ�������Ӽ��ͷ���, ��ʱ����̳и���
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
	int pass_time = 0; // �ѹ�ʱ��
	int wait_time = 0; // �ȴ�ʱ��
	bool is_paused = false; // �Ƿ���ͣ
	bool is_triggered = false; // �Ƿ񴥷�
	bool is_one_shot = false; // �Ƿ񵥴δ���
	std::function<void()> callback;
};

