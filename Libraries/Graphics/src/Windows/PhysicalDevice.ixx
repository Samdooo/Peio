export module Peio.Graphics.Windows.PhysicalDevice;

export import Peio.Graphics.PhysicalDevice;
export import Peio.Essentials.Types;

export namespace Peio {

	namespace Graphics {

		namespace Windows {

			class PhysicalDevice : public Graphics::PhysicalDevice {

				struct FamilyIndices {
					uint graphics;
				} familyIndices{};

				bool suitable = true;

			public:

				void Init();

				bool IsSuitable() const;

			};

		}
		using namespace Windows;

	}
	using namespace Graphics;

}