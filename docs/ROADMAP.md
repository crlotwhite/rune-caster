# Rune Caster Roadmap

> **Last updated:** <!-- YYYY-MM-DD -->

---

## 1. Current Status (2025-06)

* Core Unicode **Rune/RuneSequence** model and basic *spell* pipeline (case conversion, whitespace normalization, punctuation filter)
* Initial language detection module (rule-based)
* C++20 modules prototype (`rune_caster_concepts.cppm`)
* Cross-platform CMake build + unit tests (🏠 Windows / Linux)
* GitHub Actions: Doxygen documentation → GitHub Pages ✅

---

## 2. Short-Term Targets (0-3 months)

| ID | Item | Outcome |
|---|---|---|
| S-1 | **CI Fix:** Clang 20 × `spdlog`/fmt `constexpr/consteval` incompatibility | 100 % green CI matrix |
| S-2 | **Ranges Integration:** expose `RuneSequence` & `Spell` pipeline as `std::ranges` views/adaptors | Seamless STL interop |
| S-3 | **Python Binding:** `pybind11` bindings, publish *rune-caster* PyPI package | Broader adoption in ML pipelines |
| S-4 | **CLI Tool:** `rune-caster` command (YAML/JSON pipeline, benchmark mode) | Quick experimentation |

---

## 3. Mid-Term Targets (3-6 months)

| ID | Item | Outcome |
|---|---|---|
| M-1 | **Advanced Tokenizers:** Korean (KoNLPy/KKMA), Japanese (MeCab) integration | Multilingual segmentation |
| M-2 | **Plugin System:** load `spell_*` as shared libraries at runtime | Extensibility w/o recompilation |
| M-3 | **Packaging:** Conan & vcpkg manifests | Easy consumption by downstream projects |
| M-4 | **CI Enhancements:** macOS ARM64, Windows ARM64, fuzzing (libFuzzer) | Robustness & coverage |

---

## 4. Long-Term Targets (6-12 months)

| ID | Item | Outcome |
|---|---|---|
| L-1 | **Vectorisation:** AVX/NEON acceleration for heavy-text loops | 2-5× throughput |
| L-2 | **ML Language Detection:** ONNX Runtime backend | Accuracy & adaptability |
| L-3 | **WebAssembly Build:** Browser & Node support | Web demos & edge runtimes |
| L-4 | **Visual Pipeline Debugger:** graph-based UI showing intermediate tokens | Developer experience |

---

## 5. Nice-to-Have Ideas

* GraphQL/REST API microservice example
* Emscripten playground on docs site
* Mermaid diagrams for internal architecture

---

## 6. Tracking & Issue Mapping

| Roadmap ID | GitHub Issue # | Title | Priority |
|---|---|---|---|
| S-1 | [#1](https://github.com/crlotwhite/rune-caster/issues/1) | Fix CI: Clang 20 × spdlog/fmt constexpr/consteval incompatibility | High |
| S-2 | [#2](https://github.com/crlotwhite/rune-caster/issues/2) | Ranges Integration: std::ranges views/adaptors for RuneSequence & Spell | High |
| S-3 | [#3](https://github.com/crlotwhite/rune-caster/issues/3) | Python Binding: pybind11 bindings + PyPI package | High |
| S-4 | [#4](https://github.com/crlotwhite/rune-caster/issues/4) | CLI Tool: rune-caster command with YAML/JSON pipeline | Medium |
| M-1 | [#5](https://github.com/crlotwhite/rune-caster/issues/5) | Advanced Tokenizers: Korean (KoNLPy/KKMA) & Japanese (MeCab) integration | Medium |
| M-2 | [#6](https://github.com/crlotwhite/rune-caster/issues/6) | Plugin System: Dynamic loading of spell_* shared libraries | Medium |
| M-3 | [#7](https://github.com/crlotwhite/rune-caster/issues/7) | Packaging: Conan & vcpkg manifests for easy consumption | Medium |
| M-4 | [#8](https://github.com/crlotwhite/rune-caster/issues/8) | CI Enhancements: macOS ARM64, Windows ARM64, fuzzing | Medium |
| L-1 | [#9](https://github.com/crlotwhite/rune-caster/issues/9) | Vectorization: AVX/NEON acceleration for heavy-text processing | Low |
| L-2 | [#10](https://github.com/crlotwhite/rune-caster/issues/10) | ML Language Detection: ONNX Runtime backend for accuracy | Low |
| L-3 | [#11](https://github.com/crlotwhite/rune-caster/issues/11) | WebAssembly Build: Browser & Node.js support | Low |
| L-4 | [#12](https://github.com/crlotwhite/rune-caster/issues/12) | Visual Pipeline Debugger: Graph-based UI for token visualization | Low |

## 7. Issue Templates

아래는 각 로드맵 항목에 대한 GitHub Issue 템플릿입니다. 수동으로 Issues를 생성할 때 참고하세요.

### Short-Term Issues (S-1 ~ S-4)
- **Labels**: `enhancement`, `priority:high`, 관련 기술 태그
- **Assignees**: 프로젝트 메인테이너
- **Milestone**: v1.0.0 또는 해당 릴리스

### Mid-Term Issues (M-1 ~ M-4)  
- **Labels**: `enhancement`, `priority:medium`, 관련 기술 태그
- **Milestone**: v1.1.0 또는 해당 릴리스

### Long-Term Issues (L-1 ~ L-4)
- **Labels**: `enhancement`, `priority:low`, `future`
- **Milestone**: v2.0.0 또는 해당 릴리스 