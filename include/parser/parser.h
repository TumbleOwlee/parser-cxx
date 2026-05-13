#include "error.h"
#include "input.h"
#include "result.h"
#include "stack.h"
#include "types.h"
#include "util.h"

#include <expected>

namespace parse {

    /*!
     * \brief Parses the input and returns a vector of layers if successful, or an
     * error if not.
     *
     * \param input The input to parse.
     * \return A vector of layers if parsing is successful, or an error if parsing
     * fails.
     */
    inline auto parse(Input input) -> std::expected<std::vector<parse::Layer>, parse::Error> {
        Stack stack;
        skip(input);

        std::vector<parse::Layer> values;

        while (*input != '\0') {
            parse::Result result;

#define TRY_PARSE_TYPE(T)                                                                                              \
    {                                                                                                                  \
        ::parse::Result value;                                                                                         \
        if ((value = ::parse::Type<T>::parse(stack, input))) {                                                         \
            values.push_back(std::move(stack.pop_layer()));                                                            \
            continue;                                                                                                  \
        } else if (result || result.error() < value.error()) {                                                         \
            result = std::move(value);                                                                                 \
        }                                                                                                              \
    }

            TRY_PARSE_TYPE(::parse::If);
            TRY_PARSE_TYPE(::parse::Block);
#undef TRY_PARSE_TYPE

            return std::unexpected<parse::Error>(result.error());
        }

        return values;
    }

} // namespace parse
