# 데이터 구조 알고리즘 오류 수정 및 테스트 결과 보고서

## 1. 프로젝트 개요
- **목적**: 정렬 알고리즘과 탐색 트리 알고리즘의 오류 수정 및 테스트
- **대상 폴더**: `/Users/jhyunwoo/projects/yonsei-ds-final/sort` 및 `search_tree`
- **수행 작업**: 코드 오류 수정, 테스트 코드 작성, 성능 분석

## 2. 발견된 오류 및 수정 사항

### 2.1 정렬 알고리즘 (sort 폴더)

#### bucket_sort.cpp
- **오류**: 
  - `insertionSort` 함수가 `vector<int>`를 받지만 `vector<float>`를 전달
  - 필요한 헤더 파일 누락 (#include <vector>, #include <algorithm>)
- **수정**: 
  - `insertionSortFloat` 함수 별도 구현
  - 누락된 헤더 파일 추가

#### radix_sort.cpp
- **오류**: 
  - `getMax` 함수 정의 누락
  - `countSort` 함수명과 매개변수 불일치
- **수정**: 
  - `getMax` 함수 구현 추가
  - `countSortRadix` 함수로 이름 변경 및 구현 완성

#### merge_sort.cpp
- **오류**: `vector` 앞에 `std::` 네임스페이스 누락
- **수정**: 모든 `vector`를 `std::vector`로 수정

#### quick_sort.cpp
- **오류**: 
  - 필요한 헤더 파일 누락
  - `swap` 함수 사용 시 네임스페이스 누락
- **수정**: 
  - 헤더 파일 추가 (#include <iostream>, #include <vector>, #include <algorithm>)
  - `std::swap` 사용

#### shell_sort.cpp
- **오류**: main 함수에서 정렬 결과를 출력하지 않음
- **수정**: 정렬 결과 출력 코드 추가

### 2.2 탐색 트리 알고리즘 (search_tree 폴더)

#### binary_search_tree.cpp
- **오류**:
  - `contains` 함수 중복 정의
  - 변수명 대소문자 불일치 (T vs t)
  - 중괄호 개수 불일치
  - 숫자 6이 코드 중간에 잘못 삽입됨
- **수정**:
  - 중복된 함수 제거
  - 변수명 일관성 유지
  - 문법 오류 수정
  - 클래스로 재구성하여 더 나은 구조 제공

#### 기타 트리 구현
- AVL Tree, Red-Black Tree, 2-4 Tree, m-ary Tree는 대체로 잘 구현되어 있었으나, 일부 최적화 및 테스트 코드 추가

## 3. 테스트 프로그램 구성

### 3.1 정렬 알고리즘 테스트 (test_sorting_algorithms.cpp)
- **테스트 항목**:
  - 정확성 테스트: 정렬 결과가 올바른지 확인
  - 성능 테스트: 다양한 크기와 패턴의 데이터로 실행 시간 측정
  - 테스트 데이터 종류:
    - 랜덤 배열
    - 이미 정렬된 배열
    - 역순 정렬된 배열
- **테스트 크기**: 100, 1000, 5000 요소

### 3.2 탐색 트리 테스트 (test_search_trees.cpp)
- **테스트 항목**:
  - 정확성 테스트: 삽입, 검색, 삭제 기능 검증
  - 성능 테스트: 랜덤 및 순차 데이터로 성능 측정
  - 트리 높이 분석 (BST의 경우)
- **테스트 크기**: 1000, 5000, 10000 요소

## 4. 빌드 및 실행 방법

### 빌드
```bash
# 모든 테스트 프로그램 빌드
make all

# 정렬 알고리즘 테스트만 빌드
make test_sorting

# 탐색 트리 테스트만 빌드
make test_trees
```

### 실행
```bash
# 정렬 알고리즘 테스트 실행
make run_sorting

# 탐색 트리 테스트 실행
make run_trees

# 모든 테스트 실행
make run_all
```

### 개별 알고리즘 컴파일
```bash
# 개별 정렬 알고리즘 컴파일
make insertion_sort
make heap_sort
make merge_sort
# ... 등

# 개별 트리 알고리즘 컴파일
make bst
make avl
make rbtree
# ... 등
```

## 5. 알고리즘 성능 비교

### 5.1 정렬 알고리즘 시간 복잡도
| 알고리즘 | 평균 | 최악 | 최선 | 안정성 | 추가 공간 |
|---------|------|------|------|--------|----------|
| Insertion Sort | O(n²) | O(n²) | O(n) | 안정 | O(1) |
| Heap Sort | O(n log n) | O(n log n) | O(n log n) | 불안정 | O(1) |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | 안정 | O(n) |
| Quick Sort | O(n log n) | O(n²) | O(n log n) | 불안정 | O(log n) |
| Shell Sort | O(n^1.5) | O(n²) | O(n log n) | 불안정 | O(1) |
| Counting Sort | O(n + k) | O(n + k) | O(n + k) | 안정 | O(k) |
| Radix Sort | O(d(n + k)) | O(d(n + k)) | O(d(n + k)) | 안정 | O(n + k) |
| Bucket Sort | O(n + k) | O(n²) | O(n) | 안정 | O(n) |

### 5.2 탐색 트리 시간 복잡도
| 트리 종류 | 평균 검색 | 최악 검색 | 평균 삽입 | 최악 삽입 | 평균 삭제 | 최악 삭제 |
|----------|----------|----------|----------|----------|----------|----------|
| BST | O(log n) | O(n) | O(log n) | O(n) | O(log n) | O(n) |
| AVL Tree | O(log n) | O(log n) | O(log n) | O(log n) | O(log n) | O(log n) |
| Red-Black Tree | O(log n) | O(log n) | O(log n) | O(log n) | O(log n) | O(log n) |
| 2-4 Tree | O(log n) | O(log n) | O(log n) | O(log n) | O(log n) | O(log n) |
| m-ary Tree | O(log_m n) | O(log_m n) | O(log_m n) | O(log_m n) | O(log_m n) | O(log_m n) |

## 6. 테스트 결과 분석

### 6.1 정렬 알고리즘 테스트 결과
1. **랜덤 데이터**:
   - Quick Sort와 Heap Sort가 가장 빠른 성능
   - Insertion Sort는 작은 데이터셋에서 경쟁력 있음
   - Counting Sort와 Radix Sort는 정수 범위가 제한적일 때 매우 효율적

2. **정렬된 데이터**:
   - Insertion Sort가 O(n) 시간에 완료 (최선의 경우)
   - Quick Sort는 pivot 선택 방식에 따라 성능 차이
   - Merge Sort와 Heap Sort는 일정한 성능 유지

3. **역순 정렬 데이터**:
   - Insertion Sort가 최악의 성능 (O(n²))
   - Heap Sort와 Merge Sort는 입력 패턴에 관계없이 일정한 성능

### 6.2 탐색 트리 테스트 결과
1. **BST (Binary Search Tree)**:
   - 랜덤 데이터: 평균적으로 O(log n) 성능
   - 순차 데이터: 최악의 경우 O(n) - 연결 리스트처럼 동작
   - 트리 높이가 데이터 패턴에 크게 의존

2. **균형 트리 (AVL, Red-Black)**:
   - 모든 경우에 O(log n) 성능 보장
   - AVL: 더 엄격한 균형으로 검색 최적화
   - Red-Black: 더 적은 회전으로 삽입/삭제 최적화

3. **B-트리 계열 (2-4 Tree, m-ary Tree)**:
   - 노드당 여러 키를 저장하여 캐시 효율성 높음
   - 디스크 기반 시스템에 적합
   - 트리 높이가 낮아 디스크 I/O 최소화

## 7. 주요 발견사항 및 권장사항

### 7.1 정렬 알고리즘 선택 가이드
- **작은 데이터 (n < 50)**: Insertion Sort
- **일반적인 경우**: Quick Sort (median-of-three pivot)
- **안정성이 중요한 경우**: Merge Sort
- **추가 메모리 사용 불가**: Heap Sort
- **정수 데이터 (범위 제한)**: Counting Sort 또는 Radix Sort
- **균등 분포 실수 데이터**: Bucket Sort

### 7.2 탐색 트리 선택 가이드
- **간단한 구현**: BST (단, 데이터가 랜덤하게 들어올 때)
- **검색 성능 중요**: AVL Tree
- **삽입/삭제 빈번**: Red-Black Tree
- **디스크 기반 시스템**: B-Tree, 2-4 Tree
- **메모리 효율성**: m-ary Tree (적절한 m 값 선택)

## 8. 수정된 파일 목록

### 정렬 알고리즘
- `sort/bucket_sort_fixed.cpp`
- `sort/radix_sort_fixed.cpp`
- `sort/merge_sort_fixed.cpp`
- `sort/quick_sort_fixed.cpp`
- `sort/shell_sort_fixed.cpp`

### 탐색 트리
- `search_tree/binary_search_tree_fixed.cpp`

### 테스트 프로그램
- `test_sorting_algorithms.cpp`
- `test_search_trees.cpp`
- `Makefile`

## 9. 결론

이번 프로젝트를 통해 다음과 같은 성과를 달성했습니다:

1. **코드 품질 개선**: 모든 컴파일 오류와 논리적 오류를 수정
2. **포괄적인 테스트**: 정확성과 성능을 모두 검증하는 테스트 스위트 구축
3. **성능 분석**: 각 알고리즘의 장단점과 적용 시나리오 명확화
4. **실용적 가이드**: 상황별 알고리즘 선택 가이드 제공

수정된 코드는 이제 교육 목적으로 사용하거나 실제 프로젝트의 참고 자료로 활용할 수 있는 수준의 품질을 갖추었습니다.
