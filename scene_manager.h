#pragma once
#include "scene.h"
#include "camera.h"

extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* selector_scene;

class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Selector,
		Game,
	};
public:
	SceneManager() = default;
	~SceneManager() = default;

	/// <summary>
	/// ���õ�ǰ�����ķ���һ��������Ϸ��ʼ��,
	/// ֻ�������ó�������������ڳ���ʱ�ű�����,
	/// ���ӳ�����ʵ��������ͬʱ����,������ﴫ��ָ������� 
	/// </summary>
	/// <param name="scene"></param>
	void set_current_scene(Scene* scene) {
		current_scene = scene;
		current_scene->on_enter();
	}
	
	/// <summary>
	/// ��ת�����ķ���һ�����ڲ�ͬ�ӳ����ĸ���,�ӳ���֮�����
	/// �˴˵����ÿ��ܻ������ֵ��ڴ��������,����������ʹ��ö��
	/// </summary>
	/// <param name="scene_type"></param>
	void switch_scene(SceneType scene_type) {
		current_scene->on_exit();
		switch (scene_type)
		{
		case SceneManager::SceneType::Menu:
			current_scene = menu_scene;
			break;
		case SceneManager::SceneType::Game:
			current_scene = game_scene;
			break;
		case SceneType::Selector:
			current_scene = selector_scene;
			break;
		default:
			break;
		}
		current_scene->on_enter();
	}

	void on_update(int delta) {
		current_scene->on_update(delta);
	}

	void on_draw(const Camera& camera) {
		current_scene->on_draw(camera);
	}

	void on_input(const ExMessage& msg) {
		current_scene->on_input(msg);
	}
private:
	Scene* current_scene = nullptr;
};

