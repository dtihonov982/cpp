#include <iostream>
#include <memory>

class MediaPlayer {
public:
    virtual void play(std::string audioType, std::string fileName) = 0;
};

class AdvancedMediaPlayer {
public:
    virtual void playVlc(std::string fileName) = 0;
    virtual void playMp4(std::string fileName) = 0;
};

using AdvancedMediaPlayerPtr = std::unique_ptr<AdvancedMediaPlayer>;

class VlcPlayer: public AdvancedMediaPlayer {
public:
    void playVlc(std::string fileName) override {
        std::cout << "Playing vlc file. Name: " + fileName << '\n';
    }

    void playMp4(std::string fileName) override {
    }
};

class Mp4Player: public AdvancedMediaPlayer {
public:
    void playVlc(std::string fileName) override {
    }

    void playMp4(std::string fileName) override {
        std::cout << "Playing mp4 file. Name: " + fileName << '\n';
    }
};

class MediaAdapter: public MediaPlayer {
public:
    MediaAdapter(std::string audioType) {
        if (audioType == "vlc") {
            advancedMusicPlayer = AdvancedMediaPlayerPtr{new VlcPlayer()};
        }
        else if (audioType == "mp4") {
            advancedMusicPlayer = AdvancedMediaPlayerPtr{new Mp4Player()};
        }
    }
    void play(std::string audioType, std::string fileName) override {
        if (audioType == "vlc") {
            advancedMusicPlayer->playVlc(fileName);
        }
        else if (audioType == "mp4") {
            advancedMusicPlayer->playMp4(fileName);
        }
    }
private:
    AdvancedMediaPlayerPtr advancedMusicPlayer;
};

class AudioPlayer: public MediaPlayer {
public:
    void play(std::string audioType, std::string fileName) override {
        if (audioType == "mp3") {
            std::cout << "Playing mp3 file. Name: " << fileName << '\n';
        }
        else if (audioType == "vlc" || audioType == "mp4") {
            auto mediaAdapter = std::make_unique<MediaAdapter>(audioType);
            mediaAdapter->play(audioType, fileName);
        }
        else {
            std::cout << "Invalid media. " << audioType << " format not supported.\n";
        }
    }
};

int main() {
    AudioPlayer audioPlayer;
    audioPlayer.play("mp3", "beyond the horizon.mp3");
    audioPlayer.play("mp4", "alone.mp4");
    audioPlayer.play("vlc", "far far away.vlc");
    audioPlayer.play("avi", "mind me.avi");
    return 0;
}
