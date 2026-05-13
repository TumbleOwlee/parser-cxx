#pragma once

#include "compare.h"
#include "error.h"
#include "value.h"

#include <expected>
#include <variant>
#include <vector>

namespace parse {

    /*!
     * \brief A layer is either a value or a comparison. A layer can be an argument
     * to a function, or a comparison or a return value.
     */
    class Layer : public std::variant<std::monostate, ::parse::Value, ::parse::compare::Comparison> {
      public:
        /*!
         * \brief Construct a layer from a value or a comparison.
         * \tparam T The type of the value or comparison.
         * \param value The value or comparison to construct the layer from.
         */
        template <typename T>
        Layer(T value) : std::variant<std::monostate, ::parse::Value, ::parse::compare::Comparison>(value) {}
    };

    /*!
     * \brief Output a layer to a stream.
     * \param out The stream to output the layer to.
     * \param layer The layer to output.
     * \return The stream after outputting the layer.
     */
    auto operator<<(std::ostream &out, Layer const &layer) -> std::ostream &;

    /*!
     * \brief Output a vector of layers to a stream.
     * \param out The stream to output the layers to.
     * \param result The vector of layers to output, or an error if the parsing
     * failed.
     * \return The stream after outputting the layers or the error.
     */
    auto operator<<(std::ostream &out, std::expected<std::vector<parse::Layer>, parse::Error> const &result)
        -> std::ostream &;

} // namespace parse
