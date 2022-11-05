#include "form.hpp"

namespace twang::forms {

form::form(form::type t)
	: m_type(t)
{}

form::type form::form_type() const {
	return m_type;
}

}
