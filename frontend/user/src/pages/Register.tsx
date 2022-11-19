import { Box, Button, TextField } from "@mui/material";
import { useEffect, useState } from "react";
import { useStore } from "../state/store";


export function Register() {
  const store = useStore();
  const [name, setName] = useState('');

  useEffect(() => {
    
    document.addEventListener('keydown', e => {
      if (e.key === 'Enter') {
        store.register(name);
      }
    })
  })

  const onRegister = () => {
    store.register(name);
  }
  return (
    <Box>
      <TextField value={name} onChange={e => setName(e.target.value)} label={'Name'} />
      <Button onClick={onRegister}>Sign Up</Button>
    </Box>
  )
}