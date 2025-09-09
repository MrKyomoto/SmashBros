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
	/// 设置当前场景的方法一般用于游戏初始化,
	/// 只会在设置场景管理器的入口场景时才被调用,
	/// 与子场景的实例化几乎同时进行,因此这里传参指针更方便 
	/// </summary>
	/// <param name="scene"></param>
	void set_current_scene(Scene* scene) {
		current_scene = scene;
		current_scene->on_enter();
	}
	
	/// <summary>
	/// 跳转场景的方法一般用于不同子场景的更新,子场景之间持有
	/// 彼此的引用可能会出现奇怪的内存管理问题,所以在这里使用枚举
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

