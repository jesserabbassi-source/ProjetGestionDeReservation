from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route("/chat", methods=["POST"])
def chat():
    data = request.get_json()
    question = data.get("question", "")

    # Simple demo response
    response_text = f"Vous avez dit: {question}"

    return jsonify({"response": response_text})

if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5000, debug=True)
