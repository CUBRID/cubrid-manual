document.addEventListener("DOMContentLoaded", function () {
  const select = document.getElementById("version-select");
  const remoteUrl = "https://ftp.cubrid.org/CUBRID_Docs/Manuals/switcher.json";

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

      // 현재 언어(ko 또는 en) 식별
      const langMatch = currentPath.match(/^\/manual\/(ko|en)\//);
      const currentLang = langMatch ? langMatch[1] : "ko"; // 기본 ko 처리

      data.forEach(item => {
        const option = document.createElement("option");

        // 상대 경로 추출 및 슬래시 정리
        const relativePath = currentPath.replace(/^\/manual\/(ko|en)\/[^/]+\//, "");
        const cleanPath = relativePath.replace(/^\/+/, "");

        // item.url에 언어 변경 적용
        let baseUrl = item.url.replace(/\/$/, "");
        if (currentLang === "en") {
          baseUrl = baseUrl.replace("/manual/ko/", "/manual/en/");
        }

        const newUrl = baseUrl + "/" + cleanPath + currentHash;

        option.value = newUrl;
        option.textContent = item.version;

        if (currentPath.startsWith(new URL(item.url).pathname.replace("/manual/ko/", `/manual/${currentLang}/`))) {
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
