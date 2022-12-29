package com.tes.project.adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.tes.project.R;
import com.tes.project.databinding.ItemFileBinding;

import java.util.ArrayList;
import java.util.List;

public class RvFilesAdapter extends RecyclerView.Adapter<RvFilesAdapter.MyViewHolder> {

    private Context mContext;
    private List<String> mList = new ArrayList<>();

    public RvFilesAdapter(Context mContext , List<String> mList){
this.mContext = mContext;
        this.mList = mList;

    }




    @NonNull
    @Override
    public MyViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {

        //以前写法
        //MyViewHolder holder = new MyViewHolder(LayoutInflater.from(mContext).inflate(R.layout.item_file,parent,false));

        ItemFileBinding binding = ItemFileBinding.inflate(LayoutInflater.from(parent.getContext()), parent, false);
        MyViewHolder holder = new MyViewHolder(binding);


        return holder;
    }

    @Override
    public void onBindViewHolder(@NonNull MyViewHolder holder, int position) {
        holder.binding.tvName.setText(mList.get(position));
    }

    @Override
    public int getItemCount() {
        return mList.size();
    }

    class MyViewHolder extends  RecyclerView.ViewHolder{

        ItemFileBinding binding;

        public MyViewHolder(@NonNull ItemFileBinding binding) {
            super(binding.getRoot());
            this.binding = binding;
        }
    }





}
