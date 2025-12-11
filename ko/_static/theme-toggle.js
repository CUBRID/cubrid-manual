(function() {
  // #left-sidebar 찾기
  var sidebar = document.getElementById('left-sidebar');
  if (!sidebar) {
    console.error('[ERROR] #left-sidebar 요소를 찾을 수 없습니다.');
    return;
  }
  console.log('[OK] #left-sidebar 발견');

  // 부모 container (grid 컨테이너) 찾기
  var container = sidebar.parentElement;
  if (!container) {
    console.error('[ERROR] container를 찾을 수 없습니다.');
    return;
  }
  console.log('[OK] container 발견:', container.className);

  // 중복 방지: 이미 resizer가 있으면 종료
  if (document.getElementById('sidebar-grid-resizer')) {
    console.warn('[WARN] Resizer가 이미 존재합니다.');
    return;
  }

  // Breakpoint별 초기 크기 정의
  function getInitialWidth() {
    var windowWidth = window.innerWidth;
    if (windowWidth >= 1024) {
      return 240; // lg: breakpoint
    } else if (windowWidth >= 768) {
      return 220; // md: breakpoint
    } else {
      return 220; // 기본값 (모바일에서는 사용 안함)
    }
  }

  // localStorage에서 이전 width 가져오기 (없으면 breakpoint 기준)
  var savedWidth = localStorage.getItem('sidebarGridWidth');
  var width = savedWidth ? parseInt(savedWidth, 10) : getInitialWidth();
  var defaultWidth = getInitialWidth();
  var iconModeWidth = 24;
  var isIconMode = false;

  console.log('[INFO] 사이드바 초기 너비:', width + 'px (breakpoint 기준: ' + getInitialWidth() + 'px)');

  // 데스크톱 모드일 때만 grid 적용 함수
  function applyGridWidth() {
    if (window.innerWidth >= 768) {
      container.style.gridTemplateColumns = width + 'px minmax(0, 1fr)';
    } else {
      container.style.gridTemplateColumns = '';
    }
  }

  // 아이콘 모드 체크 및 전환
  function checkIconMode() {
    if (window.innerWidth >= 768) {
      if (width <= 98) {
        if (!isIconMode) {
          isIconMode = true;
          width = iconModeWidth;
          sidebar.classList.add('icon-mode');
          applyGridWidth();
          console.log('[INFO] 아이콘 모드 활성화 (24px)');
        }
      } else {
        if (isIconMode) {
          isIconMode = false;
          sidebar.classList.remove('icon-mode');
          console.log('[INFO] 아이콘 모드 비활성화');
        }
      }
    } else {
      if (isIconMode) {
        isIconMode = false;
        sidebar.classList.remove('icon-mode');
        console.log('[INFO] 모바일 모드 - 아이콘 모드 해제');
      }
    }
  }

  // 초기 적용
  applyGridWidth();
  checkIconMode();

  // 아이콘 모드 스타일 + 스크롤바 영역 보존 스타일 추가
  var iconModeStyle = document.createElement('style');
  iconModeStyle.textContent = `
    @media (min-width: 768px) {
      /* 스크롤바 공간 항상 확보 */
      #left-sidebar {
        scrollbar-gutter: stable;
      }
      
      #left-sidebar.icon-mode {
        overflow: hidden;
      }
      #left-sidebar.icon-mode > div:not(#sidebar-expand-icon),
      #left-sidebar.icon-mode > nav,
      #left-sidebar.icon-mode > a,
      #left-sidebar.icon-mode > button {
        opacity: 0;
        pointer-events: none;
        transition: opacity 0.2s;
      }
      #left-sidebar.icon-mode #sidebar-expand-icon {
        opacity: 0.6 !important;
        pointer-events: auto !important;
      }
      
      /* Resizer hover 시 스크롤 비활성화 (영역은 유지) */
      #left-sidebar.resizer-hover {
        pointer-events: none;
        user-select: none;
      }
    }
    
    @media (max-width: 767px) {
      #left-sidebar.icon-mode > div,
      #left-sidebar.icon-mode > nav,
      #left-sidebar.icon-mode > a,
      #left-sidebar.icon-mode > button {
        opacity: 0.6 !important;
        pointer-events: auto !important;
      }
      #left-sidebar.icon-mode #sidebar-expand-icon {
        display: none !important;
      }
    }
  `;
  document.head.appendChild(iconModeStyle);

  // 확장 아이콘 생성
  var expandIcon = document.createElement('div');
  expandIcon.id = 'sidebar-expand-icon';
  expandIcon.innerHTML = `
    <svg width="16" height="16" viewBox="0 0 16 16" fill="none" xmlns="http://www.w3.org/2000/svg">
      <path d="M3 8h10M8 3l5 5-5 5" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
    </svg>
  `;
  expandIcon.style.cssText = `
    position: absolute;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
    cursor: pointer;
    padding: 4px;
    border-radius: 4px;
    transition: background 0.2s, opacity 0.2s;
    z-index: 10;
    opacity: 0;
    pointer-events: none;
    display: flex;
    align-items: center;
    justify-content: center;
  `;

  expandIcon.addEventListener('mouseenter', function() {
    if (window.innerWidth >= 768) {
      this.style.background = 'rgba(59, 130, 246, 0.1)';
    }
  });
  expandIcon.addEventListener('mouseleave', function() {
    this.style.background = 'transparent';
  });

  expandIcon.addEventListener('click', function(e) {
    if (window.innerWidth < 768) return;
    e.stopPropagation();
    width = getInitialWidth();
    isIconMode = false;
    sidebar.classList.remove('icon-mode');
    applyGridWidth();
    localStorage.setItem('sidebarGridWidth', width);
    console.log('[INFO] 사이드바 복원:', width + 'px (breakpoint 기준)');
  });

  sidebar.appendChild(expandIcon);

  // Resizer 핸들 생성 - 4px, sidebar와 main 사이에 독립적으로 배치
  var resizer = document.createElement('div');
  resizer.id = 'sidebar-grid-resizer';
  resizer.style.position = 'absolute';
  resizer.style.left = '-3rem';  // sidebar 우측 border 바로 옆
  resizer.style.top = '0';
  resizer.style.width = '4px';
  resizer.style.height = '100%';
  resizer.style.cursor = 'ew-resize';
  resizer.style.zIndex = '999';
  resizer.style.background = 'transparent';
  resizer.style.transition = 'background 0.2s, opacity 0.2s';
  resizer.style.transform = 'translateX(0)';  // 정확한 위치 제어

  // Container를 relative로 설정
  if (getComputedStyle(container).position === 'static') {
    container.style.position = 'relative';
  }

  // 모바일에서는 숨김 처리
  function updateResizerVisibility() {
    if (window.innerWidth >= 768) {
      resizer.style.display = 'block';
      resizer.style.pointerEvents = 'auto';
    } else {
      resizer.style.display = 'none';
      resizer.style.pointerEvents = 'none';
    }
  }

  updateResizerVisibility();

  // hover 효과 - 스크롤바 영역은 유지하면서 스크롤 기능만 비활성화
  resizer.addEventListener('mouseenter', function() {
    if (window.innerWidth >= 768) {
      //this.style.background = 'rgba(59, 130, 246, 0.6)';
      // 스크롤 인터랙션만 비활성화 (영역은 유지)
      sidebar.classList.add('resizer-hover');
      console.log('[INFO] Resizer hover - 스크롤 인터랙션 비활성화 (영역 유지)');
    }
  });
  
  resizer.addEventListener('mouseleave', function() {
    if (window.innerWidth >= 768 && !isResizing) {
      //this.style.background = 'transparent';
      // 스크롤 인터랙션 복원
      sidebar.classList.remove('resizer-hover');
      console.log('[INFO] Resizer leave - 스크롤 인터랙션 복원');
    }
  });

  // Resizer를 sidebar 다음(main 이전)에 삽입
  var mainElement = sidebar.nextElementSibling;
  if (mainElement) {
    // main 요소를 relative로 설정하고 resizer를 내부에 배치
    if (getComputedStyle(mainElement).position === 'static') {
      mainElement.style.position = 'relative';
    }
    mainElement.insertBefore(resizer, mainElement.firstChild);
    console.log('[OK] Resizer 핸들 추가됨 (4px, main 내부 좌측에 배치 - sidebar border 바로 옆)');
  } else {
    console.error('[ERROR] main 요소를 찾을 수 없습니다.');
    return;
  }

  // === 드래그 기능 ===
  var isResizing = false;

  resizer.addEventListener('mousedown', function(e) {
    if (window.innerWidth < 768) return;

    isResizing = true;
    document.body.style.cursor = 'ew-resize';
    document.body.style.userSelect = 'none';
    //this.style.background = 'rgba(59, 130, 246, 0.8)';
    // 드래그 중에는 스크롤 인터랙션 비활성화
    sidebar.classList.add('resizer-hover');
    e.preventDefault();

    console.log('[INFO] 드래그 시작');
  });

  document.addEventListener('mousemove', function(e) {
    if (!isResizing || window.innerWidth < 768) return;

    var containerLeft = container.getBoundingClientRect().left;
    var newWidth = e.clientX - containerLeft;

    newWidth = Math.max(24, Math.min(600, newWidth));

    if (newWidth <= 98) {
      container.style.gridTemplateColumns = newWidth + 'px minmax(0, 1fr)';
      width = newWidth;
    } else {
      container.style.gridTemplateColumns = newWidth + 'px minmax(0, 1fr)';
      width = newWidth;
      if (isIconMode) {
        isIconMode = false;
        sidebar.classList.remove('icon-mode');
      }
    }
  });

  document.addEventListener('mouseup', function(e) {
    if (isResizing) {
      isResizing = false;
      document.body.style.cursor = '';
      document.body.style.userSelect = '';
      //resizer.style.background = 'transparent';

      if (window.innerWidth >= 768 && width <= 98) {
        width = iconModeWidth;
        isIconMode = true;
        sidebar.classList.add('icon-mode');
        applyGridWidth();
        console.log('[INFO] 아이콘 모드 활성화 (24px)');
      }

      // 스크롤 인터랙션 복원
      sidebar.classList.remove('resizer-hover');
      console.log('[INFO] 드래그 종료 - 스크롤 인터랙션 복원');

      localStorage.setItem('sidebarGridWidth', width);
      console.log('[OK] 사이드바 너비 저장됨:', width + 'px');
    }
  });

  // 윈도우 리사이즈 이벤트
  var resizeTimer;
  window.addEventListener('resize', function() {
    clearTimeout(resizeTimer);
    resizeTimer = setTimeout(function() {
      defaultWidth = getInitialWidth();
      applyGridWidth();
      updateResizerVisibility();
      checkIconMode();
      console.log('[INFO] 화면 크기 변경 감지 - 너비:', window.innerWidth + 'px, breakpoint 기준:', defaultWidth + 'px');
    }, 150);
  });

  console.log('[SUCCESS] 사이드바 리사이저 설치 완료! (4px resizer, sidebar와 main 사이 배치, 스크롤바 영역 보존)');
})();
