document.addEventListener("DOMContentLoaded", function () {
    address_split = window.location.pathname.split("/", 2).filter(Boolean);;
    fetch_address = "";
    link_template = "";
    re_template = "";
    if (address_split.length == 1) {
        if (address_split[0] == "docs") {
            fetch_address = "/docs_versions.json";
            link_template = "/docs/[version]/";
            re_template = /\/docs\/([^\/]+)/;
        } else if (address_split[0] == "dev") {
            fetch_address = "/dev_versions.json";
            link_template = "/dev/[version]/docs";
            re_template = /\/dev\/([^\/]+)\/docs/;
        }
    }

    const versionSelectBlock = document.getElementById("dd3d_version_block");
    if (versionSelectBlock) {
        versionSelectBlock.hidden = true;
    }

    if (fetch_address) {
        fetch(fetch_address)
            .then(response => response.json())
            .then(versions => {
                // Get docs/ version
                const currentVersionMatch = window.location.pathname.match(re_template);
                const currentVersion = currentVersionMatch ? currentVersionMatch[1] : null;

                const versionSelect = document.getElementById("dd3d_version_select");
                versionSelect.innerHTML = "";

                versions.forEach(version => {
                    const option = document.createElement("option");
                    option.value = version;
                    option.text = version;

                    if (version === currentVersion) {
                        option.selected = true;
                    }
                    versionSelect.appendChild(option);
                });

                versionSelectBlock.hidden = false;
                versionSelect.addEventListener("change", function () {
                    const selectedVersion = versionSelect.value;
                    window.location.href = link_template.replace("[version]", selectedVersion);
                });
            })
            .catch(error => {
                console.error("Error loading JSON file:", error);
            });
    }
});