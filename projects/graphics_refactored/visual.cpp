#include "visual.hpp"

win::WinInstance::WinInstance(sf::RenderWindow& window) : window_(window) {
  window_.clear(sf::Color::White);
  image_.create(window.getSize().x, window.getSize().y, sf::Color::White);
  texture_.loadFromImage(image_);
  sprite_.setTexture(texture_);
  sprite_.setPosition(0, 0);
}

void win::WinInstance::drawImage() {
  window_.clear(sf::Color::White);
  texture_.update(image_);
  sprite_.setTexture(texture_);
  window_.draw(sprite_);
  window_.display();
}

void win::WinInstance::display() {
  window_.display();
}

bool win::WinInstance::isOpen() const {
  return window_.isOpen();
}

bool win::WinInstance::pollEvent(sf::Event& event) {
  return window_.pollEvent(event);
}

void win::WinInstance::close() {
  window_.close();
}

void win::WinInstance::clear(sf::Color color) {
  window_.clear(color);
}

void win::WinInstance::saveToFile(const std::filesystem::path& path) {
  image_.saveToFile(path);
}