# 🛡️ Rune Caster Security Setup

이 문서는 GitHub 저장소에서 코드 스캐닝과 보안 기능을 활성화하는 방법을 간략히 설명합니다.

## 1. Code Scanning 설정

1. 저장소 **Settings** 탭으로 이동합니다.
2. **Code security and analysis** 섹션에서 **Code scanning**을 활성화합니다.
3. **Set up CodeQL**을 선택하여 기본 워크플로(`codeql.yml`)를 생성하고 커밋합니다.
4. 워크플로가 병합되면 GitHub Actions가 푸시마다 코드를 분석하여 잠재적인 취약점을 보고합니다.

## 2. 추가 보안 기능

- **Dependabot alerts/security updates**: 의존성 취약점이 발견되면 PR 형태로 패치를 제안합니다.
- **Secret scanning**: 토큰이나 비밀 값이 리포지토리에 포함되는지 감지합니다.
- 워크플로 권한을 제한하고, 브랜치 보호 규칙에서 CodeQL 검사를 필수 조건으로 설정하면 더욱 안전합니다.

이 과정을 통해 Rune Caster 프로젝트는 GitHub의 기본 보안 기능을 활용하여 잠재적인 위험을 조기에 발견하고 대응할 수 있습니다.
