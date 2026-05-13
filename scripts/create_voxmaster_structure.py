#!/usr/bin/env python3
"""Create the Phase 1 VoxMaster project scaffold.

The script is intentionally idempotent: existing files are left untouched, while
missing directories and placeholder files are created.
"""
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

DIRECTORIES = [
    "external/onnxruntime",
    "models",
    "resources/calibration",
    "resources/presets",
    "Source/core",
    "Source/dsp",
    "Source/ai",
    "Source/reference",
    "Source/feedback",
    "Source/ui",
]

FILES = [
    "CMakeLists.txt",
    "Source/PluginProcessor.h",
    "Source/PluginProcessor.cpp",
    "Source/PluginEditor.h",
    "Source/PluginEditor.cpp",
    "Source/core/DropOldestSPSCQueue.h",
    "Source/core/LockFreeLogQueue.h",
    "Source/core/LoggerThread.h",
    "Source/core/LoggerThread.cpp",
    "Source/core/SharedState.h",
    "Source/core/SharedState.cpp",
    "Source/core/ParameterSmoother.h",
    "Source/core/SCurveSmoother.h",
    "Source/core/ParameterSnapshot.h",
    "Source/dsp/DSPManager.h",
    "Source/dsp/DSPManager.cpp",
    "Source/dsp/InputCalibrationProcessor.h",
    "Source/dsp/InputCalibrationProcessor.cpp",
    "Source/dsp/EQProcessor.h",
    "Source/dsp/EQProcessor.cpp",
    "Source/dsp/DynamicEQProcessor.h",
    "Source/dsp/DynamicEQProcessor.cpp",
    "Source/dsp/DeEsserProcessor.h",
    "Source/dsp/DeEsserProcessor.cpp",
    "Source/dsp/CompressorProcessor.h",
    "Source/dsp/CompressorProcessor.cpp",
    "Source/dsp/ToneMatchProcessor.h",
    "Source/dsp/ToneMatchProcessor.cpp",
    "Source/dsp/SafetyLimiter.h",
    "Source/dsp/SafetyLimiter.cpp",
    "Source/dsp/FeatureExtractor.h",
    "Source/dsp/FeatureExtractor.cpp",
    "Source/ai/InferenceEngine.h",
    "Source/ai/InferenceEngine.cpp",
    "Source/ai/InferenceThread.h",
    "Source/ai/InferenceThread.cpp",
    "Source/ai/FeatureFrame.h",
    "Source/ai/ModelOutput.h",
    "Source/ai/ReferenceProfile.h",
    "Source/ai/TonalBalanceMatcher.h",
    "Source/ai/TonalBalanceMatcher.cpp",
    "Source/reference/ReferenceAnalyzer.h",
    "Source/reference/ReferenceAnalyzer.cpp",
    "Source/reference/ReferenceAnalysisThread.h",
    "Source/reference/ReferenceAnalysisThread.cpp",
    "Source/feedback/FeedbackRecorder.h",
    "Source/feedback/FeedbackRecorder.cpp",
    "Source/feedback/FeedbackWriterThread.h",
    "Source/feedback/FeedbackWriterThread.cpp",
    "Source/feedback/FeedbackRecord.h",
    "Source/ui/MainComponent.h",
    "Source/ui/MainComponent.cpp",
    "Source/ui/HeaderBar.h",
    "Source/ui/SpectrumView.h",
    "Source/ui/EQCurveView.h",
    "Source/ui/DynamicEQView.h",
    "Source/ui/DeEsserView.h",
    "Source/ui/CompressorView.h",
    "Source/ui/ReferenceMatchView.h",
    "Source/ui/CalibrationView.h",
    "Source/ui/AIPanel.h",
]

HEADER_TEMPLATE = "#pragma once\n\n// Phase 1 placeholder for the VoxMaster scaffold.\n"
SOURCE_TEMPLATE = "// Phase 1 placeholder for the VoxMaster scaffold.\n"


def main() -> None:
    for directory in DIRECTORIES:
        (ROOT / directory).mkdir(parents=True, exist_ok=True)

    for file_name in FILES:
        path = ROOT / file_name
        path.parent.mkdir(parents=True, exist_ok=True)
        if path.exists():
            continue
        if path.suffix in {".h", ".hpp"}:
            path.write_text(HEADER_TEMPLATE, encoding="utf-8")
        elif path.suffix in {".cpp", ".cc", ".cxx"}:
            path.write_text(SOURCE_TEMPLATE, encoding="utf-8")
        else:
            path.touch()

    print(f"VoxMaster Phase 1 scaffold ensured at {ROOT}")


if __name__ == "__main__":
    main()
