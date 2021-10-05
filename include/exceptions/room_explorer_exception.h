//
// Created by Jack Lee on 2020/11/19.
//

#ifndef NONEUCLIDEAN_RAY_CASTER_ROOM_EXPLORER_EXCEPTION_H
#define NONEUCLIDEAN_RAY_CASTER_ROOM_EXPLORER_EXCEPTION_H

#include <exception>

namespace room_explorer {

namespace exceptions {

class GeneralExplorerException : public std::exception {

};

} // namespace exceptions

} // namespace room_explorer

#endif //NONEUCLIDEAN_RAY_CASTER_ROOM_EXPLORER_EXCEPTION_H

#include <exceptions/no_room_template_exception.h>
#include <exceptions/invalid_direction_exception.h>