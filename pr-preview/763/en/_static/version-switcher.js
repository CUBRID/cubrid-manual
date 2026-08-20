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
      const currentOrigin = window.location.origin;

      const langMatch = currentPath.match(/^\/manual\/(internal\/)?(ko|en)\//);
      const currentLang = langMatch ? langMatch[2] : "ko";
      const isInternal = langMatch && langMatch[1] === "internal/";
      const internalSegment = isInternal ? "internal/" : "";

      const relativePath = currentPath.replace(
        new RegExp(`^/manual/${internalSegment}${currentLang}/[^/]+/`),
        ""
      );
      const cleanPath = relativePath.replace(/^\/+/, "");

      const currentUrl = currentOrigin + currentPath + currentHash;

      data.forEach(item => {
        const option = document.createElement("option");

        let baseUrl = item.url.replace(/\/$/, "");
        const urlObj = new URL(baseUrl);

        urlObj.hostname = new URL(currentOrigin).hostname;

        urlObj.pathname = urlObj.pathname.replace(
          /^\/manual\/(internal\/)?(ko|en)\//,
          `/manual/${internalSegment}${currentLang}/`
        );

        baseUrl = urlObj.origin + urlObj.pathname;

        const newUrl = baseUrl + "/" + cleanPath + currentHash;
        option.value = newUrl;
        option.setAttribute("data-base-url", baseUrl); // 기본 페이지용
        option.textContent = item.version;

        if (newUrl === currentUrl || decodeURIComponent(newUrl) === decodeURIComponent(currentUrl)) {
          option.selected = true;
        }

        select.appendChild(option);
      });

      select.addEventListener("change", function () {
        const targetUrl = this.value;
        const baseUrl = this.selectedOptions[0].getAttribute("data-base-url");

        if (targetUrl) {
          fetch(targetUrl, { method: "HEAD" })
            .then(response => {
              if (response.ok) {
                window.location.href = targetUrl;
              } else {
                window.location.href = baseUrl;
              }
            })
            .catch(() => {
              window.location.href = baseUrl;
            });
        }
      });
    })
    .catch(error => {
      console.error("버전 정보를 불러오는 데 실패했습니다:", error);
      select.innerHTML = "<option>버전 로딩 실패</option>";
    });
});
