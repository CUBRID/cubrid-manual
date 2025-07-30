document.addEventListener("DOMContentLoaded", function () {
  const select = document.getElementById("version-select");
  const remoteUrl = switcherUrl;

  fetch(remoteUrl)
    .then(response => {
      if (!response.ok) {
        throw new Error(`HTTP 오류: ${response.status}`);
      }
      return response.json();
    })
    .then(data => {
      select.innerHTML = ""; // 기존 옵션 초기화

      const currentPath = window.location.pathname;
      const currentHash = window.location.hash;

      // 현재 언어 및 internal 여부 추출
      const langMatch = currentPath.match(/^\/manual\/(internal\/)?(ko|en)\//);
      const currentLang = langMatch ? langMatch[2] : "ko";
      const internalSegment = langMatch && langMatch[1] ? "internal/" : "";

      // 상대 경로 추출
      const relativePath = currentPath.replace(
        new RegExp(`^/manual/${internalSegment}${currentLang}/[^/]+/`),
        ""
      );
      const cleanPath = relativePath.replace(/^\/+/, "");

      const currentUrl = window.location.origin + currentPath + currentHash;

      data.forEach(item => {
        const option = document.createElement("option");

        // baseUrl 언어 및 internal 경로 적용
        let baseUrl = item.url.replace(/\/$/, "");
        baseUrl = baseUrl.replace(
          /\/manual\/(internal\/)?ko\//,
          `/manual/${internalSegment}${currentLang}/`
        );

        const newUrl = baseUrl + "/" + cleanPath + currentHash;
        option.value = newUrl;
        option.textContent = item.version;

        // 현재 URL과 일치하면 선택
        if (newUrl === currentUrl || decodeURIComponent(newUrl) === decodeURIComponent(currentUrl)) {
          option.selected = true;
        }

        select.appendChild(option);
      });

      select.addEventListener("change", function () {
        const targetUrl = this.value;
        if (targetUrl) {
          window.location.href = targetUrl;
        }
      });
    })
    .catch(error => {
      console.error("버전 정보를 불러오는 데 실패했습니다:", error);
      select.innerHTML = "<option>버전 로딩 실패</option>";
    });
});
