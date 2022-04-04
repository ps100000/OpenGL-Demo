#pragma once

enum class elm_special{
	NONE
};

class special_base{
	public:
		static const elm_special special_type = elm_special::NONE;
};
