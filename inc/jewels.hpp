#ifndef JEWELS_HPP_INCLUDED
#define JEWELS_HPP_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

/// Enumeration representing different types of jewels.
enum JewelsType {
    BLACK , WHITE, BLUE, RED, PINK, UNKOWN
};

/// Class representing a jewel.
class Jewels {
public:

    /// Default constructor for Jewels class.
    Jewels() = default;
    
    /// Constructor for Jewels class.
    ///
    /// @param type The type of the jewel.
    Jewels(const JewelsType& type);

    /// Get the type of the jewel.
    ///
    /// @return The type of the jewel.
    auto getJewelType() -> JewelsType;

    /// Set the type of the jewel.
    ///
    /// @param type The type of the jewel.
    void setJewelType(const JewelsType& type);

    /// Destroy the jewel.
    void destroy();

    /// Check if the jewel is valid.
    ///
    /// @return True if the jewel is valid, false otherwise.
    bool getValid() const;
    
private:
    bool isValid{true}; 
    JewelsType m_type{JewelsType::UNKOWN}; 
};

#endif
