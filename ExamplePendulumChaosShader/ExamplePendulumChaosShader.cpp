#include "BBE/BrotBoxEngine.h"
#include <iostream>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
const static bbe::Vector2 middle = bbe::Vector2{ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };

class MyGame : public bbe::Game
{
public:
	bbe::FragmentShader shader;
	bbe::List<bbe::Vector2> magnets;

	virtual void onStart() override
	{
		shader.load(BBE_APPLICATION_ASSET_PATH "/fragPCS.spv");
		magnets.add({ middle + bbe::Vector2(200, 0).rotate(bbe::Math::TAU / 3.f * 0.f) });
		magnets.add({ middle + bbe::Vector2(200, 0).rotate(bbe::Math::TAU / 3.f * 1.f) });
		magnets.add({ middle + bbe::Vector2(200, 0).rotate(bbe::Math::TAU / 3.f * 2.f) });
	}

	virtual void update(float timeSinceLastFrame) override
	{
		std::cout << "FPS: " << (1 / timeSinceLastFrame) << std::endl;

		if (isMouseDown(bbe::MouseButton::LEFT))
		{
			bbe::Vector2 mousePos = getMouse();
			bbe::Vector2* closest = bbe::Math::getClosest(mousePos, magnets);
			*closest = mousePos;
		}
	}
	virtual void draw3D(bbe::PrimitiveBrush3D & brush) override
	{
	}
	virtual void draw2D(bbe::PrimitiveBrush2D & brush) override
	{
		static float magnetDistance = 50;
		static int32_t maxIter = 255;
		static float tickTime = 0.02f;
		static float power = 1;
		static float magnetStrength = 80000;
		ImGui::DragFloat("Magnet Distance: ", &magnetDistance);
		ImGui::DragInt("Max Iter", &maxIter);
		ImGui::DragFloat("Tick Time: ", &tickTime, 0.0001f);
		ImGui::DragFloat("Power: ", &power, 0.0001f);
		ImGui::DragFloat("MagnetStrength: ", &magnetStrength, 100);
		shader.setPushConstant(brush,  80, sizeof(bbe::Vector2) * magnets.getLength(), magnets.getRaw());
		shader.setPushConstant(brush, 104, sizeof(float), &magnetDistance);
		shader.setPushConstant(brush, 108, sizeof(int32_t), &maxIter);
		shader.setPushConstant(brush, 112, sizeof(float), &tickTime);
		shader.setPushConstant(brush, 116, sizeof(float), &power);
		shader.setPushConstant(brush, 120, sizeof(float), &magnetStrength);
		brush.fillRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, &shader);
	}
	virtual void onEnd() override
	{
	}
};

int main()
{
	MyGame *mg = new MyGame();
	mg->start(WINDOW_WIDTH, WINDOW_HEIGHT, "Pendulum Chaos!");
	delete mg;
}
