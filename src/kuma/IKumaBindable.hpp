#ifndef IKUMABINDABLE_HPP
#define IKUMABINDABLE_HPP

namespace life::kuma {
	class IKumaBindable {
	public:
		// Slot is "optional" depending on the implementation specfics
		virtual void Bind(unsigned int slot) const = 0; 
		virtual void Unbind() const = 0;
	};
}

#endif // IKUMABINDABLE_HPP
