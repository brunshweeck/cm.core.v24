//
// Created by brunshweeck on 6 juin 2024.
//

#include <core/concurrent/ConcurrentLinkedQueue.h>
#include <core/random/Random.h>
#include <core/lang/Enum.h>

using namespace core;
using namespace core::util;
using namespace core::concurrent;
using namespace core::random;

int main() {

  CORE_ALIAS(Category, Character::Category);

  using A = Class<Category>::Object;
  using E = Class<A>::Prime;

  E e = core::Character::LETTER_NUMBER;

  XString xs;
  xs.append(e);

  return 0;
}
