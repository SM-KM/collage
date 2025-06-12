#include "../requests/requests.h"
#include "play.h"
#include "argHandlers.h"
#include "types.h"
#include "videos.h"
#include <variant>

void PlayContentById(int id) {
  auto result = getContent(VideoType::SERIE, Flags::POPULAR, true, id);
  std::visit(
      [](auto &&content) {
        using T = std::decay_t<decltype(content)>;
        if constexpr (std::is_same_v<T, Series> || std::is_same_v<T, Movie>) {
          LoadVideoSearch(content.m_name);
        } else {
          std::cerr << "Not able to play content type";
        }
      },
      result);
}
