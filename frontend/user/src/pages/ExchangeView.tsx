import { Button, TextField, Typography } from "@mui/material";
import { useState } from "react";
import { useNavigate, useParams } from "react-router-dom";
import { Container } from "../components/Container";
import { OrderType } from "../generated/events";
import { useStore } from "../state/store";

export type BuyOrSell = "buy" | "sell";
const MAX_SHARES_TRADE = 100000;

const currentPrice = 38.32;

interface IProps {
	type: BuyOrSell;
}

export function ExchangeView(props: IProps) {
	let { id } = useParams<{ id: string }>();
	const [shares, setShares] = useState<number>(1);
	const [bid, setBid] = useState<number>(currentPrice);
	const store = useStore();

	const navigate = useNavigate();
	
  if (!id || !store.staticListings.some(lst => lst.ticker === id)) {
		// invalid stock
		navigate("/listings");
	}

	const { type } = props;

	return (
		<Container title={`${type === "buy" ? "BUY" : "SELL"} ${id}`} navigationPosition="listings" >
			<Typography
				sx={{ display: 'inline', alignItems: "left" }}
				component="span"
				variant="h6"
				color="text.primary"
			>
				{`Total: ${((bid * shares) || 0).toFixed(2)}$`}
			</Typography>
			<br></br>
			<br></br>
			<TextField
				type="number"
				style={{ width: 400 }}
				value={shares}
				onChange={evt => {
					const num = Number.parseInt(evt.target.value);
					if (isNaN(num)) setShares(0);
					else {
						if (num <= MAX_SHARES_TRADE) setShares(num);
						else setShares(MAX_SHARES_TRADE);
					}
				}}
				InputProps={{
					inputProps: {
						min: 0
					}
				}}
				label="Number of Shares"
			/>
			<br></br>
			<br></br>
			<TextField
				type="number"
				style={{ width: 400 }}
				value={bid}
				onChange={evt => {
					const num = parseFloat(evt.target.value);
					if (isNaN(num)) setBid(0);
					else {
						if (num <= 10 * currentPrice) setBid(num);
						else setBid(MAX_SHARES_TRADE);
					}
				}}
				InputProps={{
					inputProps: {
						min: 0,
						step: "0.01"
					}
				}}
				label={`${type === "buy" ? "Bid price" : "Ask price"}`}
			/>
			<br></br>
			<Typography
				sx={{ display: 'inline', alignItems: "left" }}
				component="span"
				variant="body1"
				color="text.secondary"
			>
				{`Price per Share: ${currentPrice}$`}
			</Typography>
			<br></br>
			<Button
				variant="contained"
				style={{ marginTop: 40 }}
				size="large"
				color={type === "buy" ? "success" : "error"}
				onClick={() => {
					store.placeOrder({
						amount: shares,
						price: bid,
						ticker: id as string,
						type: type === "buy" ? OrderType.BID : OrderType.ASK,
					})
				}}
			>
				{type === "buy" ? "BUY NOW" : "SELL NOW"}
			</Button>
		</Container>
	);
}
