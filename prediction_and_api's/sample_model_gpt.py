# Setup your api key:
openai.api_key = "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"

# Define the model you want to use
model_engine = "davinci:ft-personal-2023-02-13-15-53-20"

print("Starting conversation...\n")

# Starting the conversation loop
while True:
    prompt = input("You: ")

    # Use the completions API to get a response
    response = openai.Completion.create(
        engine=model_engine,
        prompt=prompt,
        max_tokens=100,
        n=1,
        stop=None,
        temperature=0.5,
    )
    print("\nModel: ", response.choices[0].text)
