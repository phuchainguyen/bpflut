# bpflut - Flutter Build and Publish Tool

A command-line tool for building and publishing Flutter applications to iOS App Store and Google Play Store.

## Project Structure

\`\`\`
bpflut/
├── src/                    # Source files
│   ├── main.c             # Main application entry point
│   ├── utils/             # Utility modules
│   │   ├── logger.c       # Logging functionality
│   │   └── config.c       # Configuration parser
│   └── commands/          # Command implementations
│       ├── init.c         # Init command dispatcher
│       ├── init_ios.c     # iOS initialization
│       ├── init_android.c # Android initialization
│       ├── build.c        # Build command dispatcher
│       ├── build_ios.c    # iOS build implementation
│       ├── build_android.c# Android build implementation
│       ├── publish.c      # Publish command dispatcher
│       ├── publish_ios.c  # iOS publish implementation
│       └── publish_android.c # Android publish implementation
├── include/               # Header files
│   ├── utils/
│   │   ├── logger.h
│   │   └── config.h
│   └── commands/
│       ├── init.h
│       ├── build.h
│       └── publish.h
├── build/                 # Build artifacts (created during build)
├── bin/                   # Executables (created during build)
├── Makefile
└── README.md
\`\`\`

## Features

- **init**: Initialize project structure with platform-specific configuration
- **build**: Build Flutter apps for iOS, Android, or both platforms
- **publish**: Publish apps to App Store Connect and Google Play Console
- Beautiful colored logging with timestamps
- Automatic build number generation
- Certificate and provisioning profile management for iOS
- Google Play Console API integration for Android

## Installation

\`\`\`bash
make
sudo make install
\`\`\`

## Usage

### Initialize Project
\`\`\`bash
bpflut init
\`\`\`

This creates `ios/` and `android/` directories with configuration templates.

### Build Applications
\`\`\`bash
# Build for all platforms (default)
bpflut build

# Build for specific platform
bpflut build ios
bpflut build android
\`\`\`

### Publish Applications
\`\`\`bash
# Publish to all stores (default)
bpflut publish

# Publish to specific store
bpflut publish ios
bpflut publish android
\`\`\`

## Configuration

After running `bpflut init`, edit the configuration files:

- `ios/config.env` - iOS build and App Store settings
- `android/config.env` - Android build and Google Play settings

### iOS Configuration
- P12 certificate and password
- Provisioning profile
- App Store Connect API key
- Team ID and Bundle ID

### Android Configuration
- Keystore file and passwords
- Google Play Console service account JSON
- Package name and release track

## Requirements

### iOS
- Xcode command line tools
- Valid Apple Developer account
- App Store Connect API key

### Android
- Flutter SDK
- bundletool (install via `brew install bundletool`)
- Google Play Console service account
- Python packages: `google-auth`, `google-api-python-client`

## Development

### Build
\`\`\`bash
make                # Release build
make debug          # Debug build with symbols
\`\`\`

### Clean
\`\`\`bash
make clean          # Remove build artifacts
\`\`\`

### Install/Uninstall
\`\`\`bash
sudo make install   # Install to /usr/local/bin
sudo make uninstall # Remove from /usr/local/bin
