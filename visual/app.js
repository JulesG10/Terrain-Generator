function getPoints(text = "") {
    let result = { points: [], max: 0 };

    let lines = text.split('\n');
    lines.forEach(line => {
        let tempArr = [];
        line.replace(/\s/g, '|').split('|').forEach(char => {

            let value = parseInt(char);
            if (!isNaN(value)) {
                result.max = value > result.max ? value : result.max;
                tempArr.push(value);
            }
        })
        if (tempArr.length > 0) {
            result.points.push(tempArr);
        }
    });
    return result;
}

new Promise(function (resolve, reject) {
    let xhr = new XMLHttpRequest();
    let url = "http://127.0.0.1:5500/test/1_100x100.txt";
    xhr.open("GET", url, true);
    xhr.onreadystatechange = function () {
        if (xhr.readyState === 4 && xhr.status === 200) {
            resolve(xhr.responseText);
        }
    };
    xhr.send({});
}).then(data => {

    let points = getPoints(data);
    console.log(points);

    let canvas = document.getElementsByTagName('canvas')[0];
    canvas.height = points.points.length;
    canvas.width = points.points.length;
    let w = canvas.width;
    let h = canvas.height;

    let ctx = canvas.getContext('2d');


    ctx.fillStyle = "rgba(" + 0 + "," + 0 + "," + 0 + "," + 100 + ")";
    ctx.fillRect(0, 0, w, h)
    let image = ctx.getImageData(0, 0, w, h);

    let y = 0;
    let x = 0;
    for (let i = 0; i < image.data.length; i += 4) {

        if (!(points.points[y] == undefined || points.points[y][x] == undefined)) {
            image.data[i] = 0;
            image.data[i + 1] = (points.points[y][x] / points.max) * 255;
            image.data[i + 2] = 0;
        }

        x++;
        if (((i / 100) / 4) % 1 == 0) {
            x = 0;
            y++;
        }
    }

    ctx.putImageData(image, 0, 0);
})